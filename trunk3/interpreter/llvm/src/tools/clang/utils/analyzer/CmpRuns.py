#!/usr/bin/env python

"""
CmpRuns - A simple tool for comparing two static analyzer runs to determine
which reports have been added, removed, or changed.

This is designed to support automated testing using the static analyzer, from
two perspectives: 
  1. To monitor changes in the static analyzer's reports on real code bases, for
     regression testing.

  2. For use by end users who want to integrate regular static analyzer testing
     into a buildbot like environment.

Usage:

    # Load the results of both runs, to obtain lists of the corresponding
    # AnalysisDiagnostic objects.
    #
    # root - the name of the root directory, which will be disregarded when 
    # determining the source file name
    # 
    resultsA = loadResults(dirA, opts, root, deleteEmpty)
    resultsB = loadResults(dirB, opts, root, deleteEmpty)
    
    # Generate a relation from diagnostics in run A to diagnostics in run B 
    # to obtain a list of triples (a, b, confidence). 
    diff = compareResults(resultsA, resultsB)
           
"""

import os
import plistlib

#
class AnalysisDiagnostic:
    def __init__(self, data, report, htmlReport):
        self._data = data
        self._loc = self._data['location']
        self._report = report
        self._htmlReport = htmlReport

    def getFileName(self):
        return self._report.run.getSourceName(self._report.files[self._loc['file']])

    def getLine(self):
        return self._loc['line']
        
    def getColumn(self):
        return self._loc['col']

    def getCategory(self):
        return self._data['category']

    def getDescription(self):
        return self._data['description']

    def getIssueIdentifier(self) :
        id = ''
        if 'issue_context' in self._data :
          id += self._data['issue_context'] + ":"
        if 'issue_hash' in self._data :
          id += str(self._data['issue_hash']) + ":"
        return id + ":" + self.getFileName()

    def getReport(self):
        if self._htmlReport is None:
            return " "
        return os.path.join(self._report.run.path, self._htmlReport)

    def getReadableName(self):
        return '%s:%d:%d, %s: %s' % (self.getFileName(), self.getLine(), 
                                     self.getColumn(), self.getCategory(), 
                                     self.getDescription())

class multidict:
    def __init__(self, elts=()):
        self.data = {}
        for key,value in elts:
            self[key] = value
    
    def __getitem__(self, item):
        return self.data[item]
    def __setitem__(self, key, value):
        if key in self.data:
            self.data[key].append(value)
        else:
            self.data[key] = [value]
    def items(self):
        return self.data.items()
    def values(self):
        return self.data.values()
    def keys(self):
        return self.data.keys()
    def __len__(self):
        return len(self.data)
    def get(self, key, default=None):
        return self.data.get(key, default)
    
#

class CmpOptions:
    def __init__(self, verboseLog=None, rootA="", rootB=""):
        self.rootA = rootA
        self.rootB = rootB
        self.verboseLog = verboseLog

class AnalysisReport:
    def __init__(self, run, files):
        self.run = run
        self.files = files

class AnalysisRun:
    def __init__(self, path, root, opts):
        self.path = path
        self.root = root
        self.reports = []
        self.diagnostics = []
        self.opts = opts

    def getSourceName(self, path):
        if path.startswith(self.root):
            return path[len(self.root):]
        return path

def loadResults(path, opts, root = "", deleteEmpty=True):
    run = AnalysisRun(path, root, opts)
    
    for f in os.listdir(path):
        if (not f.startswith('report') or
            not f.endswith('plist')):
            continue

        p = os.path.join(path, f)
        data = plistlib.readPlist(p)

        # Ignore/delete empty reports.
        if not data['files']:
            if deleteEmpty == True:
                os.remove(p)
            continue

        # Extract the HTML reports, if they exists.
        if 'HTMLDiagnostics_files' in data['diagnostics'][0]:
            htmlFiles = []
            for d in data['diagnostics']:
                # FIXME: Why is this named files, when does it have multiple
                # files?
                assert len(d['HTMLDiagnostics_files']) == 1
                htmlFiles.append(d.pop('HTMLDiagnostics_files')[0])
        else:
            htmlFiles = [None] * len(data['diagnostics'])
            
        report = AnalysisReport(run, data.pop('files'))
        diagnostics = [AnalysisDiagnostic(d, report, h) 
                       for d,h in zip(data.pop('diagnostics'),
                                      htmlFiles)]

        assert not data

        run.reports.append(report)
        run.diagnostics.extend(diagnostics)

    return run

def cmpAnalysisDiagnostic(d) :
    return d.getIssueIdentifier()

def compareResults(A, B):
    """
    compareResults - Generate a relation from diagnostics in run A to
    diagnostics in run B.

    The result is the relation as a list of triples (a, b, confidence) where
    each element {a,b} is None or an element from the respective run, and
    confidence is a measure of the match quality (where 0 indicates equality,
    and None is used if either element is None).
    """

    res = []

    # Quickly eliminate equal elements.
    neqA = []
    neqB = []
    eltsA = list(A.diagnostics)
    eltsB = list(B.diagnostics)
    eltsA.sort(key = cmpAnalysisDiagnostic)
    eltsB.sort(key = cmpAnalysisDiagnostic)
    while eltsA and eltsB:
        a = eltsA.pop()
        b = eltsB.pop()
        if (a.getIssueIdentifier() == b.getIssueIdentifier()) :
            res.append((a, b, 0))
        elif a._data > b._data:
            neqA.append(a)
            eltsB.append(b)
        else:
            neqB.append(b)
            eltsA.append(a)
    neqA.extend(eltsA)
    neqB.extend(eltsB)

    # FIXME: Add fuzzy matching. One simple and possible effective idea would be
    # to bin the diagnostics, print them in a normalized form (based solely on
    # the structure of the diagnostic), compute the diff, then use that as the
    # basis for matching. This has the nice property that we don't depend in any
    # way on the diagnostic format.

    for a in neqA:
        res.append((a, None, None))
    for b in neqB:
        res.append((None, b, None))

    return res

def dumpScanBuildResultsDiff(dirA, dirB, opts, deleteEmpty=True):
    # Load the run results.
    resultsA = loadResults(dirA, opts, opts.rootA, deleteEmpty)
    resultsB = loadResults(dirB, opts, opts.rootB, deleteEmpty)
    
    # Open the verbose log, if given.
    if opts.verboseLog:
        auxLog = open(opts.verboseLog, "wb")
    else:
        auxLog = None

    diff = compareResults(resultsA, resultsB)
    foundDiffs = 0
    for res in diff:
        a,b,confidence = res
        if a is None:
            print "ADDED: %r" % b.getReadableName()
            foundDiffs += 1
            if auxLog:
                print >>auxLog, ("('ADDED', %r, %r)" % (b.getReadableName(),
                                                        b.getReport()))
        elif b is None:
            print "REMOVED: %r" % a.getReadableName()
            foundDiffs += 1
            if auxLog:
                print >>auxLog, ("('REMOVED', %r, %r)" % (a.getReadableName(),
                                                          a.getReport()))
        elif confidence:
            print "CHANGED: %r to %r" % (a.getReadableName(),
                                         b.getReadableName())
            foundDiffs += 1
            if auxLog:
                print >>auxLog, ("('CHANGED', %r, %r, %r, %r)" 
                                 % (a.getReadableName(),
                                    b.getReadableName(),
                                    a.getReport(),
                                    b.getReport()))
        else:
            pass

    TotalReports = len(resultsB.diagnostics)
    print "TOTAL REPORTS: %r" % TotalReports
    print "TOTAL DIFFERENCES: %r" % foundDiffs
    if auxLog:
        print >>auxLog, "('TOTAL NEW REPORTS', %r)" % TotalReports
        print >>auxLog, "('TOTAL DIFFERENCES', %r)" % foundDiffs
        
    return foundDiffs    

def main():
    from optparse import OptionParser
    parser = OptionParser("usage: %prog [options] [dir A] [dir B]")
    parser.add_option("", "--rootA", dest="rootA",
                      help="Prefix to ignore on source files for directory A",
                      action="store", type=str, default="")
    parser.add_option("", "--rootB", dest="rootB",
                      help="Prefix to ignore on source files for directory B",
                      action="store", type=str, default="")
    parser.add_option("", "--verbose-log", dest="verboseLog",
                      help="Write additional information to LOG [default=None]",
                      action="store", type=str, default=None,
                      metavar="LOG")
    (opts, args) = parser.parse_args()

    if len(args) != 2:
        parser.error("invalid number of arguments")

    dirA,dirB = args

    dumpScanBuildResultsDiff(dirA, dirB, opts)    

if __name__ == '__main__':
    main()
