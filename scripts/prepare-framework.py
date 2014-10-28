#!/usr/bin/python

# https://bugreports.qt-project.org/browse/QTBUG-38511?focusedCommentId=256497&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-256497
# Please note that Qt5 frameworks have incorrect layout after SDK build, so this isn't just a problem with `macdeployqt` but whole framework assembly part.
# Present
# 	QtCore.framework/
# 		Contents/
# 			Info.plist
# 		QtCore    -> Versions/Current/QtCore
# 		Versions/
# 			Current -> 5
# 			5/
# 				QtCore
# Expected
# 	QtCore.framework/
# 		QtCore    -> Versions/Current/QtCore
# 		Resources -> Versions/Current/Resources
# 		Versions/
# 			Current -> 5
# 			5/
# 				QtCore
# 				Resources/
# 					Info.plist
# So in order to comply with expected layout: https://developer.apple.com/library/mac/documentation/MacOSX/Conceptual/BPFrameworks/Concepts/FrameworkAnatomy.html

import argparse
import os
import fnmatch
import shutil
import sys

parser = argparse.ArgumentParser(
    description = "Fixup Qt5 frameworks after macdeployqt"
)

parser.add_argument("qt5Path",action="store",
                    help="QT5 path")

parser.add_argument("bundlePath",action="store",
                    help="Bundle path (.app)")

args = parser.parse_args()

bundlePath     = os.path.abspath(args.bundlePath)
qt5Path        = os.path.abspath(args.qt5Path)
frameworksPath = os.path.join(bundlePath,'Contents','Frameworks')

def symlink(source,target):
    if not os.path.exists(target):
        print 'link: ',source,'-->',target
        os.symlink(source,target)

def move(source,target):
    print 'move: ',source,'-->',target
    shutil.move(source,target)

def copy(source,target):
    print 'copy: ',source,'-->',target
    shutil.copyfile(source,target)

for root,frameworks,files in os.walk(frameworksPath):
    for framework in frameworks:
        if fnmatch.fnmatch(framework,'Qt*.framework'):
            os.chdir(os.path.join(root,framework))
            module = framework.replace('.framework','')
            symlink('5','Versions/Current')
            symlink('Versions/Current/' + module,module)
            move('Resources','Versions/5')
            copy(os.path.join(qt5Path,'lib',framework,'Contents','Info.plist'),
               'Versions/5/Resources/Info.plist')
            symlink('Versions/Current/Resources','Resources')
