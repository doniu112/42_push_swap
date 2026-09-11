#!/usr/bin/env python3
"""Tester push_swap, Python 3.10+, bez dodatkowych bibliotek.

python3 test.py                         # wszystkie permutacje do n=6
python3 test.py --quick                 # krotszy zestaw
python3 test.py --checker ./checker_linux --valgrind
python3 test.py ./push_swap --seed 123 --samples 30

Kod wyjscia: 0 = testy OK, 1 = wykryto blad, 2 = konfiguracja,
130 = przerwanie. Progi wydajnosci sa informacyjne; --strict-performance
wymaga <2000 / <12000 dla strategii domyslnej (adaptive).
Liczby moga byc pogrupowane w argumentach, rozdzielone bialymi znakami ASCII.
Puste grupy sa bledem. Flagi musza poprzedzac liczby; dwa selektory sa bledem.
Opcja --norminette sprawdza rekurencyjnie pliki .c/.h w --source-dir (domyslnie .).
Valgrind bada osobny zestaw, a nie wszystkie permutacje.
"""
import argparse
from collections import Counter, defaultdict
import itertools
import json
import math
import os
from pathlib import Path
import random
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import time
import xml.etree.ElementTree as ET

STRATEGIES = ('simple', 'medium', 'complex', 'adaptive')
OPS = ('sa', 'sb', 'ss', 'pa', 'pb', 'ra', 'rb', 'rr', 'rra', 'rrb', 'rrr')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def simulate(values, output):
    """Niezalezny model stosow; wymaga dokladnego formatu instrukcji."""
    if output:
        require(output.endswith(b'\n'), 'Brak koncowego znaku newline na stdout')
    a, b = list(values), []
    operations = output.split(b'\n')[:-1] if output else []
    counts = Counter()
    for raw in operations:
        op = raw.decode('ascii')
        require(op in OPS, f'Niedozwolona instrukcja: {raw!r}')
        counts[op] += 1
        if op in ('pa', 'pb'):
            src, dst = (b, a) if op == 'pa' else (a, b)
            if src:
                dst.insert(0, src.pop(0))
            continue
        stacks = [a, b] if op in ('ss', 'rr', 'rrr') else (
            [a] if op in ('sa', 'ra', 'rra') else [b])
        for stack in stacks:
            if len(stack) < 2:
                continue
            if op in ('sa', 'sb', 'ss'):
                stack[0], stack[1] = stack[1], stack[0]
            elif op in ('ra', 'rb', 'rr'):
                stack.append(stack.pop(0))
            else:
                stack.insert(0, stack.pop())
    require(a == sorted(values) and not b,
            f'Nieposortowane A lub niepuste B: A={a[:20]}, B={b[:20]}')
    require(values != sorted(values) or not operations,
            'Program emituje ruchy dla posortowanego wejscia')
    return counts


def check_bench(values, strategy, stderr, counts):
    text = stderr.decode('ascii')
    n = len(values)
    inversions = sum(x > y for i, x in enumerate(values) for y in values[i+1:])
    pairs = n * (n - 1) // 2
    disorder = inversions / pairs if pairs else 0.0
    scaled = int(disorder * 10000 + 0.5)
    chosen = strategy
    if strategy == 'adaptive':
        chosen = 'simple' if disorder < .2 else 'medium' if disorder < .5 else 'complex'
    complexity = {'simple': 'O(n^2)', 'medium': 'O(n sqrt(n))',
                  'complex': 'O(n log(n))'}[chosen]
    expected = [
        f'[bench] disorder: {scaled//100}.{scaled%100:02d}%',
        f'[bench] strategy: {strategy.capitalize()} / {complexity}',
        f'[bench] total_ops: {sum(counts.values())}',
        '[bench]' + ''.join(f' {op}: {counts[op]}' for op in OPS[:5]),
        '[bench]' + ''.join(f' {op}: {counts[op]}' for op in OPS[5:]),
    ]
    # Format raportu dopasowany do ft_bench.c tego projektu.
    require(text == '\n'.join(expected) + '\n',
            'Niepoprawny bench (disorder, strategia, zlozonosc, liczniki lub format)'
            f'\nOczekiwano: {expected!r}\nOtrzymano: {text!r}')


def parse_valgrind_xml(raw):
    """Valgrind moze wpisac do XML znaki kontrolne z argv.

    XML 1.0 nie dopuszcza m.in. VT (0x0b) i FF (0x0c).
    Zmieniamy jedynie ich reprezentacje w raporcie, nigdy argumenty programu.
    Pozostale bledy skladni XML nadal powoduja blad testu.
    """
    forbidden = set(range(32)) - {9, 10, 13}
    escaped = b''.join(
        (f'\\x{byte:02x}'.encode('ascii') if byte in forbidden else bytes([byte]))
        for byte in raw
    )
    return ET.fromstring(escaped)


class Runner:
    def __init__(self, config):
        self.cfg = config
        self.groups = defaultdict(Counter)
        self.failures = []
        self.performance = defaultdict(list)
        self.started = time.monotonic()
        self.total = 0
        self.norm_results = []
        self.checker_runs = 0
        self.current = ''
        self.color = sys.stdout.isatty() and not config.no_color and 'NO_COLOR' not in os.environ

    def paint(self, text, code):
        return f'\033[{code}m{text}\033[0m' if self.color else text

    def finish_group(self):
        if self.current:
            c = self.groups[self.current]
            badge = self.paint('OK', '32') if not c['fail'] else self.paint('FAIL', '31')
            prefix = '\r' if sys.stdout.isatty() else ''
            print(f'{prefix}[{badge}] {self.current:<30} {c["pass"]:>5} OK | {c["fail"]:>3} bledow' + ' '*8,
                  flush=True)

    def process(self, command, timeout, data=None):
        return subprocess.run(command, input=data, capture_output=True, timeout=timeout)

    def case(self, group, args, values=None, strategy='adaptive', bench=False, memory=False):
        if group != self.current:
            self.finish_group()
            self.current = group
        args = list(map(str, args))
        command = [self.cfg.binary, *args]
        p = None
        valgrind_xml = None
        try:
            if memory:
                with tempfile.TemporaryDirectory(prefix='push-swap-vg-') as folder:
                    xml = Path(folder) / 'valgrind.xml'
                    vg = [self.cfg.valgrind_path, '--tool=memcheck', '--leak-check=full',
                          '--show-leak-kinds=all', '--errors-for-leak-kinds=all',
                          '--track-origins=yes', '--error-exitcode=99', '--xml=yes',
                          f'--xml-file={xml}', *command]
                    p = self.process(vg, self.cfg.valgrind_timeout)
                    require(xml.exists(), 'Valgrind nie utworzyl raportu XML')
                    raw_xml = xml.read_bytes()
                    valgrind_xml = raw_xml.decode('utf-8', errors='replace')
                    root = parse_valgrind_xml(raw_xml)
                    errors = root.findall('error')
                    finished = any(s.findtext('state') == 'FINISHED' for s in root.findall('status'))
                    require(finished, 'Valgrind nie zakonczyl analizy')
                    require(not errors and p.returncode != 99,
                            'Valgrind: ' + '; '.join(ET.tostring(e, encoding='unicode') for e in errors))
            else:
                p = self.process(command, self.cfg.timeout)
            if values is None:
                require(p.returncode > 0 and p.returncode != 99,
                        f'Oczekiwano kontrolowanego bledu, kod={p.returncode}')
                require(p.stdout == b'' and p.stderr == b'Error\n',
                        'Bledne dane musza dawac tylko Error\\n na stderr')
            else:
                require(p.returncode == 0, f'Nieprawidlowy kod zakonczenia: {p.returncode}')
                counts = simulate(values, p.stdout)
                if bench:
                    check_bench(values, strategy, p.stderr, counts)
                else:
                    require(not p.stderr, 'Nieoczekiwany tekst na stderr')
                if self.cfg.checker and values:
                    checked = self.process([self.cfg.checker, *map(str, values)],
                                           self.cfg.timeout, p.stdout)
                    self.checker_runs += 1
                    require(checked.returncode == 0 and checked.stdout == b'OK\n' and not checked.stderr,
                            f'Checker: kod={checked.returncode}, stdout={checked.stdout!r}, stderr={checked.stderr!r}')
                if group == 'Losowe i wydajnosc':
                    self.performance[(len(values), strategy)].append(sum(counts.values()))
            self.groups[group]['pass'] += 1
        except (ValueError, OSError, subprocess.TimeoutExpired, ET.ParseError) as error:
            self.groups[group]['fail'] += 1
            self.failures.append({'group': group, 'args': args, 'memory': memory,
                                  'command': shlex.join(command), 'error': str(error),
                                  'valgrind_xml': valgrind_xml,
                                  'returncode': p.returncode if p else None,
                                  'stdout': p.stdout.decode(errors='replace') if p else None,
                                  'stderr': p.stderr.decode(errors='replace') if p else None})
        self.total += 1
        if sys.stdout.isatty() and self.total % 20 == 0:
            print(f'\r[....] {group:<30} wykonano {self.total} testow', end='', flush=True)

    def check_norminette(self):
        self.finish_group()
        self.current = 'Norminette'
        files = sorted(p for p in Path(self.cfg.source_dir).rglob('*')
                       if p.is_file() and p.suffix in ('.c', '.h'))
        require(files, 'Brak plikow .c/.h do sprawdzenia')
        print(f'Norminette: {len(files)} plikow w {self.cfg.source_dir}', flush=True)
        for path in files:
            command = [self.cfg.norminette_path, str(path)]
            result = {'file': str(path), 'command': shlex.join(command),
                      'returncode': None, 'stdout': '', 'stderr': '', 'status': 'PASS'}
            try:
                process = self.process(command, self.cfg.norminette_timeout)
                result.update(returncode=process.returncode,
                              stdout=process.stdout.decode(errors='replace'),
                              stderr=process.stderr.decode(errors='replace'))
                output = result['stdout'] + result['stderr']
                require(process.returncode == 0, f'Norminette: kod {process.returncode}')
                require(not re.search(r'(?m)^\s*Error(?:!|:)', output),
                        'Norminette zglosilo blad')
                require(re.search(r'(?m):\s*OK!\s*$', result['stdout']) is not None,
                        'Brak potwierdzenia OK! dla pliku')
                self.groups[self.current]['pass'] += 1
            except (ValueError, OSError, subprocess.TimeoutExpired) as error:
                result['status'] = 'FAIL'
                result['error'] = str(error)
                self.groups[self.current]['fail'] += 1
                self.failures.append({'group': self.current, 'args': [str(path)],
                                      'command': result['command'], 'memory': False,
                                      'error': str(error) + '\n' + result['stdout'] + result['stderr'],
                                      'returncode': result['returncode'],
                                      'stdout': result['stdout'], 'stderr': result['stderr'],
                                      'valgrind_xml': None})
            self.norm_results.append(result)
            self.total += 1
            badge = self.paint(result['status'], '32' if result['status'] == 'PASS' else '31')
            print(f'  [{badge}] {path.relative_to(self.cfg.source_dir)}', flush=True)

    def summary(self, interrupted=False):
        self.finish_group()
        print('\nLICZBA OPERACJI - te same zestawy dla czterech strategii')
        print(f'{"N":>5}  {"Strategia":<10} {"Min":>7} {"Srednia":>9} {"Max":>7}  Ocena najgorszej proby')
        perf_fail = False
        for (n, strategy), values in sorted(self.performance.items()):
            thresholds = (700, 1500, 2000) if n == 100 else (5500, 8000, 12000)
            worst = max(values)
            grade = next((label for limit, label in zip(thresholds, ('swietnie', 'dobrze', 'minimum'))
                          if worst < limit), 'ponad limit')
            if strategy == 'adaptive' and worst >= thresholds[-1]:
                perf_fail = True
            print(f'{n:>5}  {strategy:<10} {min(values):>7} {sum(values)/len(values):>9.1f} {worst:>7}  {grade}')
        for i, failure in enumerate(self.failures[:10], 1):
            print(self.paint(f'\nBLAD {i}: {failure["group"]}', '31'))
            print(failure['error'][:1500])
            if any(ord(c) < 32 for arg in failure['args'] for c in arg):
                print('Argumenty (znaki specjalne): ' + repr(failure['args']))
            command = failure['command']
            print('Powtorz: ' + (command if len(command) < 700 else command[:700] + ' ... [pelne w JSON]'))
            if failure['memory']:
                print('Uruchom powyzsze polecenie przez valgrind --leak-check=full.')
        failed = bool(self.failures) or (self.cfg.strict_performance and perf_fail)
        status = 'PRZERWANO' if interrupted else 'FAIL' if failed else 'PASS'
        print('\n' + '=' * 72)
        print(self.paint(f'{status} | {self.total-len(self.failures)} OK | {len(self.failures)} bledow'
                         f' | {time.monotonic()-self.started:.1f} s', '31' if failed or interrupted else '32'))
        print(f'Checker: {self.checker_runs} wywolan' if self.cfg.checker else 'Checker zewnetrzny: nie wlaczono; uzyto symulatora Python.')
        print('Valgrind: osobny zestaw wykonany (szczegoly powyzej).' if self.cfg.valgrind
              else 'Valgrind: nie wlaczono; brak potwierdzenia braku wyciekow.')
        print(f'Norminette: {len(self.norm_results)} plikow sprawdzonych.' if self.cfg.norminette
              else 'Norminette: nie wlaczono.')
        print('Wydajnosc: ' + ('wymagane minimum dla adaptive.' if self.cfg.strict_performance
                               else 'informacyjna, nie zmienia wyniku PASS/FAIL.'))
        if self.cfg.strict_performance and perf_fail:
            print('FAIL: adaptive przekroczyl wymagany limit operacji.')
        report = {'status': status, 'seed': self.cfg.seed, 'binary': self.cfg.binary,
                  'checker': self.cfg.checker, 'valgrind': self.cfg.valgrind,
                  'norminette': self.norm_results,
                  'configuration': vars(self.cfg), 'tests': self.total,
                  'elapsed_seconds': time.monotonic()-self.started,
                  'groups': dict(self.groups), 'failures': self.failures,
                  'performance': [{'n': n, 'strategy': s, 'operations': v}
                                  for (n, s), v in sorted(self.performance.items())]}
        Path(self.cfg.report).write_text(json.dumps(report, indent=2, ensure_ascii=False), encoding='utf-8')
        print(f'Raport: {Path(self.cfg.report).resolve()}')
        return 130 if interrupted else int(failed)


def invalid_cases():
    cases = [[''], ['+'], ['-'], ['abc'], ['1.5'], ['++1'], ['+-1'], ['0x10'], ['2147483648'],
             ['-2147483649'], ['9'*10000], ['-'+'9'*10000], ['1', '1'],
             ['+0', '-0'], ['01', '1'], ['abc', '3', '2', '1'], ['2', '3', '2', '1'],
             ['--unknown', '1'], ['--bench'], ['2', '--bench', '1']]
    cases += [[' '], ['\t\n'], ['1', ''], ['', '1'], ['1', '  '],
              ['1 2', '2'], ['1 2', '3 01'], ['+0 -0'], ['1 2x 3'],
              ['1 - 2'], ['1 ++2'], ['2147483648 1'], ['1 -2147483649'],
              ['--simple 2 1'], ['1', '--bench 2'], ['1,2'], ['1\u00a02']]
    cases += [['--'+s] for s in STRATEGIES]
    cases += [['--'+a, '--'+b, '2', '1'] for a in STRATEGIES for b in STRATEGIES]
    cases += [['--'+a, '--bench', '--'+b, '2', '1'] for a in STRATEGIES for b in STRATEGIES]
    return cases


def grouped_cases(seed):
    cases = [([' 1'], [1]), (['1 '], [1]), (['1\t'], [1]),
             (['1\n'], [1]), (['1 2'], [1, 2]),
             (['5 4 3 2 1'], [5, 4, 3, 2, 1]),
             (['5 4', '3', '2 1'], [5, 4, 3, 2, 1]),
             (['1 2', '3', '4 5'], [1, 2, 3, 4, 5]),
             ([' +01\t-002\n0\r '], [1, -2, 0]),
             (['3\v2\f1'], [3, 2, 1]),
             (['-2147483648 2147483647', '0'], [-2147483648, 2147483647, 0])]
    rng = random.Random(seed)
    for size in (5, 20, 100):
        values = rng.sample(range(-10000, 10000), size)
        args = []
        i = 0
        while i < size:
            width = rng.randint(1, 5)
            args.append(' \t' + ' \n'.join(map(str, values[i:i+width])) + '\r ')
            i += width
        cases.append((args, values))
    return cases


def suite(r):
    if r.cfg.norminette:
        r.check_norminette()
    r.case('Wejscie i flagi', [], [])
    for args in invalid_cases():
        r.case('Wejscie i flagi', args)
    for s in STRATEGIES:
        for flags in [['--'+s], ['--bench', '--'+s], ['--'+s, '--bench']]:
            r.case('Wejscie i flagi', flags + ['+01', '-0', '-002'], [1, 0, -2], s, '--bench' in flags)
    for token in ['0'*10000, '0'*10000+'2147483647', '-'+'0'*10000+'2147483648']:
        r.case('Wejscie i flagi', [token], [int(token.lstrip('+-0') or '0') * (-1 if token[0] == '-' else 1)])
    for args, values in grouped_cases(r.cfg.seed):
        for strategy in STRATEGIES:
            for bench in (False, True):
                flags = ['--'+strategy] + (['--bench'] if bench else [])
                r.case('Liczby w grupach', flags + args, values, strategy, bench)
        r.case('Liczby w grupach', args, values)
        r.case('Liczby w grupach', ['--bench'] + args, values, bench=True)
    base = [[42], [1, 2, 3], [2, 1], [3, 2, 1], [2147483647, 0, -2147483648],
            list(range(30)), list(range(29, -1, -1)), list(range(1, 30))+[0],
            list(range(0, 30, 2))+list(range(1, 30, 2)), [1, 0]+list(range(2, 30))]
    # n=5: dokladne granice disorder 0.2 oraz 0.5 i ich sasiedztwo.
    for inv in (1, 2, 3, 4, 5, 6):
        base.append(next(list(v) for v in itertools.permutations(range(5))
                         if sum(x > y for i, x in enumerate(v) for y in v[i+1:]) == inv))
    for values in base:
        for s in STRATEGIES:
            for bench in (False, True):
                r.case('Przypadki szczegolne', ['--'+s]+(['--bench'] if bench else [])+values,
                       values, s, bench)
        r.case('Przypadki szczegolne', values, values)
        r.case('Przypadki szczegolne', ['--bench']+values, values, bench=True)
    for n in range(1, (4 if r.cfg.quick else 6)+1):
        for v in itertools.permutations(range(n)):
            for s in STRATEGIES:
                r.case('Permutacje', ['--'+s, '--bench', *v], list(v), s, True)
    rng = random.Random(r.cfg.seed)
    samples = r.cfg.samples if r.cfg.samples is not None else (3 if r.cfg.quick else 20)
    for n in (100, 500):
        for _ in range(samples):
            values = rng.sample(range(-1000000, 1000001), n)
            for s in STRATEGIES:
                r.case('Losowe i wydajnosc', ['--'+s, '--bench', *values], values, s, True)
    if r.cfg.valgrind:
        r.case('Pamiec: Valgrind', [], [], memory=True)
        for s in STRATEGIES:
            for v in base[:5]+[rng.sample(range(-10000, 10000), 500)]:
                r.case('Pamiec: Valgrind', ['--'+s, '--bench', *v], v, s, True, True)
        for args, values in grouped_cases(r.cfg.seed):
            r.case('Pamiec: Valgrind', ['--bench'] + args, values, bench=True, memory=True)
        for args in invalid_cases():
            r.case('Pamiec: Valgrind', args, memory=True)


def executable(path, parser):
    resolved = Path(path).expanduser().resolve()
    if not resolved.is_file() or not os.access(resolved, os.X_OK):
        parser.error(f'Brak wykonywalnego pliku: {resolved}. Zbuduj program / sprawdz chmod +x.')
    return str(resolved)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('binary', nargs='?', default='./push_swap')
    parser.add_argument('--quick', action='store_true', help='permutacje do n=4, domyslnie 3 losowania')
    parser.add_argument('--samples', type=int, help='liczba losowan dla kazdego rozmiaru')
    parser.add_argument('--seed', type=int, default=42, help='ziarno powtarzalnych losowan')
    parser.add_argument('--checker', help='sciezka do checker_linux (opcjonalnie)')
    parser.add_argument('--norminette', action='store_true', help='sprawdz rekurencyjnie pliki .c/.h')
    parser.add_argument('--source-dir', default='.', help='katalog zrodel dla Norminette')
    parser.add_argument('--norminette-timeout', type=float, default=60, help='limit sekund na plik')
    parser.add_argument('--valgrind', action='store_true', help='dodatkowy zestaw testow pamieci')
    parser.add_argument('--timeout', type=float, default=10, help='limit sekund na program / checker')
    parser.add_argument('--valgrind-timeout', type=float, default=60)
    parser.add_argument('--strict-performance', action='store_true')
    parser.add_argument('--no-color', action='store_true')
    parser.add_argument('--report', default='test_report.json', help='plik raportu (nadpisywany)')
    cfg = parser.parse_args()
    if (not math.isfinite(cfg.norminette_timeout) or cfg.norminette_timeout <= 0
            or not math.isfinite(cfg.timeout) or not math.isfinite(cfg.valgrind_timeout)
            or cfg.timeout <= 0 or cfg.valgrind_timeout <= 0
            or (cfg.samples is not None and cfg.samples < 1)):
        parser.error('Limity czasu i liczba losowan musza byc dodatnie.')
    cfg.binary = executable(cfg.binary, parser)
    if cfg.checker:
        cfg.checker = executable(cfg.checker, parser)
    cfg.valgrind_path = shutil.which('valgrind')
    if cfg.valgrind and not cfg.valgrind_path:
        parser.error('Brak Valgrinda. W WSL: sudo apt install valgrind')
    cfg.source_dir = str(Path(cfg.source_dir).expanduser().resolve())
    cfg.norminette_path = shutil.which('norminette')
    if cfg.norminette:
        if not cfg.norminette_path:
            parser.error('Brak norminette w PATH tego terminala. Sprawdz: norminette --version')
        if not Path(cfg.source_dir).is_dir():
            parser.error('Katalog zrodel nie istnieje: ' + cfg.source_dir)
        if not any(p.is_file() and p.suffix in ('.c', '.h') for p in Path(cfg.source_dir).rglob('*')):
            parser.error('Brak plikow .c/.h w katalogu zrodel.')
    protected = {Path(cfg.binary), Path(__file__).resolve()}
    if cfg.checker:
        protected.add(Path(cfg.checker))
    if Path(cfg.report).resolve() in protected:
        parser.error('Raport nie moze nadpisac testera ani programu/checkera.')
    print('='*72 + '\n PUSH_SWAP TESTER | sortowanie / walidacja / bench / pamiec\n' + '='*72)
    print(f'Program: {cfg.binary}\nTryb: {"szybki" if cfg.quick else "pelny"} | seed: {cfg.seed}\n', flush=True)
    runner = Runner(cfg)
    interrupted = False
    try:
        suite(runner)
    except KeyboardInterrupt:
        interrupted = True
        print('\nPrzerwano przez uzytkownika; zapisuje dotychczasowe wyniki.')
    try:
        return runner.summary(interrupted)
    except OSError as error:
        print(f'Nie mozna zapisac raportu: {error}', file=sys.stderr)
        return 2


if __name__ == '__main__':
    sys.exit(main())
