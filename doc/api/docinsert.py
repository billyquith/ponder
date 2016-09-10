"""
Document example processor
"""

import os
import glob

THIS_DIR = os.path.dirname(__file__)
ROOT_DIR = os.path.abspath(os.path.join(THIS_DIR, '../..'))
EG_SRC_DIR = os.path.join(ROOT_DIR, 'test/examples')
EG_USE_DIR = os.path.join(ROOT_DIR, 'doc/egs')

EG_LIST_PAGE = os.path.join(ROOT_DIR, 'doc/api/pages/_examples.hpp')
LIST_PAGE = """
/**

EGS

 */
"""

EG_FILES = []

def processDoc(fpath):
    fname = os.path.basename(fpath)
    print 'Processing', fname
    
    with open(fpath, 'r') as fh:
        txt = fh.read()
        
    egs = [] # {'name':str, 'txt':[...]}
    egname = None
    
    for ln in txt.split('\n'):
        sln = ln.strip()
        
        # //$ eg_begin,name=intro
        if sln.startswith('//$'):
            cmds = sln[3:].strip().split(',')
            
            for cmd in cmds:
                cmd = cmd.strip()
                if cmd == 'eg_begin':
                    egname,egtxt = None,[]
                elif cmd == 'eg_end':                
                    egs.append({'name':egname, 'txt':egtxt})
                    egname = None
                elif cmd.startswith('name='):
                    egname = cmd.split('=')[1]
        else:
            # only want txt in an example
            if egname:
                egtxt.append(ln)
        
    for eg in egs:
        egfname = 'eg_%s.cpp' % eg['name']
        EG_FILES.append(egfname)
        egfile = os.path.join(EG_USE_DIR, egfname)
        print 'Writing eg file:', egfile
        with open(egfile, 'w') as fh:
            txt = '\n'.join(eg['txt'])
            fh.write(txt)
        
    print 'Finished file'

def processDocs():
    for f in glob.glob(os.path.join(EG_SRC_DIR, '*.cpp')):
        processDoc(f)
        
    with open(EG_LIST_PAGE, 'w') as fh:
        txt = LIST_PAGE.replace('EGS', '\n'.join(['\\example %s' % e for e in EG_FILES]))
        fh.write(txt)
    print 'Written examples list'
    

def main():
    print '-'*60
    print 'Doc inserter'
    print '-'*60
    print 'Root dir:', ROOT_DIR
    print 'Doc examples source dir:', EG_SRC_DIR
    print 'Doc examples use dir:', EG_USE_DIR
    
    if not os.path.exists(EG_USE_DIR):
        os.mkdir(EG_USE_DIR)
        print 'Created output dir:', EG_USE_DIR
    processDocs()
    
main()

