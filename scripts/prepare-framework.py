#!/usr/bin/python

# Taken from https://gist.github.com/kingcheez/6154462d7734e0c0f3a4
#
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

# Additionally, 5.4 RC/Stable has a problem with incorrect symbolic links. See
# https://bugreports.qt-project.org/browse/QTBUG-42954

import argparse
import os
import fnmatch
import shutil
import sys
import subprocess

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
    if not os.path.exists(target):
        print 'move: ',source,'-->',target
        shutil.move(source,target)

def copy(source,target):
    if not os.path.exists(target):
        print 'copy: ',source,'-->',target
        shutil.copyfile(source,target)

def chdir(target):
    print 'chdir: ',target
    os.chdir(target)

def rmdir(target):
    print 'rmdir: ',target
    shutil.rmtree(target)

def mkdir(target):
    if not os.path.isdir(target):
        os.makedirs(target)

for root,frameworks,files in os.walk(frameworksPath):
    for framework in frameworks:
        if fnmatch.fnmatch(framework,'Qt*.framework'):
            chdir(os.path.join(root,framework))
            module = framework.replace('.framework','')
            symlink('5','Versions/Current')
            symlink('Versions/Current/' + module,module)
            move('Resources','Versions/Current/Resources')
            # rmdir('Resources')
            # mkdir('Versions/5/Resources')
            copy(os.path.join(qt5Path,'lib',framework,'Contents','Info.plist'),
               'Versions/5/Resources/Info.plist')
            symlink('Versions/Current/Resources','Resources')

def call_program(*args):
    l = subprocess.check_output(*args).strip().split("\n")
    if len(l) == 1:
        return l[0]
    else:
        return l

chdir(bundlePath)
for link in call_program(["find","-L",bundlePath,"-type","l"]):
    filename = os.path.split(link)[1]
    # find the actual file
    params = ["find",bundlePath,
              "-type","f",
              "-and","-not","-type","l",
              "-and","-name",filename]
    targetfile = call_program(params)
    os.remove(link)
    symlink(targetfile,link)
