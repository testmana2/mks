# This script configures QScintilla for PyQt v3 and/or v4.
#
# Copyright (c) 2008 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of QScintilla.
# 
# This file may be used under the terms of the GNU General Public
# License versions 2.0 or 3.0 as published by the Free Software
# Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file.  Alternatively you may (at
# your option) use any later version of the GNU General Public
# License if such license has been publicly approved by Riverbank
# Computing Limited (or its successors, if any) and the KDE Free Qt
# Foundation. In addition, as a special exception, Riverbank gives you
# certain additional rights. These rights are described in the Riverbank
# GPL Exception version 1.1, which can be found in the file
# GPL_EXCEPTION.txt in this package.
# 
# Please review the following information to ensure GNU General
# Public Licensing requirements will be met:
# http://trolltech.com/products/qt/licenses/licensing/opensource/. If
# you are unsure which license is appropriate for your use, please
# review the following information:
# http://trolltech.com/products/qt/licenses/licensing/licensingoverview
# or contact the sales department at sales@riverbankcomputing.com.
# 
# This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
# INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
# granted herein.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


import sys
import os
import glob
import getopt


# Import SIP's configuration module so that we have access to the error
# reporting.  Then try and import the configuration modules for both PyQt3 and
# PyQt4.
try:
    import sipconfig
except ImportError:
    sys.stderr.write("Unable to import sipconfig.  Please make sure SIP is installed.\n")
    sys.exit(1)

try:
    import PyQt4.pyqtconfig as pyqt4
except:
    pyqt4 = None

try:
    import pyqtconfig as pyqt3
except:
    pyqt3 = None

if pyqt4 is not None:
    pyqt = pyqt4.Configuration()
    qt_data_dir = pyqt.qt_data_dir
elif pyqt3 is not None:
    pyqt = pyqt3.Configuration()
    qt_data_dir = pyqt.qt_dir
else:
    sipconfig.error("Unable to find either PyQt v3 or v4.")


# This must be kept in sync with qscintilla.pro.
QSCI_API_MAJOR = 3


# Initialise the globals.
sip_min_version = 0x040400

opt_qscimoddir = None
opt_qscisipdir = None
opt_qsciincdir = None
opt_qscilibdir = None
opt_qscidir = None
opt_concat = False
opt_split = 1
opt_static = False
opt_tracing = False
opt_debug = False

if sys.platform == "win32":
    qsci_define = "QEXTSCINTILLA_DLL"
else:
    qsci_define = ""


def usage(rcode = 2):
    """Display a usage message and exit.

    rcode is the return code passed back to the calling process.
    """
    print "Usage:"
    print "    python configure.py [-h] [-a dir] [-c] [-d dir] [-j #] [-k] [-n dir] [-o dir] [-p 3|4] [-r] [-s] [-u] [-v dir]"
    print "where:"
    print "  -h      display this help message"
    print "  -a dir  where QScintilla's API file will be installed [default QTDIR/qsci]"
    print "  -c      concatenate the C++ source files"
    print "  -d dir  where the QScintilla module will be installed [default %s]" % pyqt.pyqt_mod_dir
    print "  -j #    split the concatenated C++ source files into # pieces [default 1]"
    print "  -k      build the QScintilla module as a static library"
    print "  -n dir  the directory containing the QScintilla Qsci header file directory [default %s]" % pyqt.qt_inc_dir
    print "  -o dir  the directory containing the QScintilla library [default %s]" % pyqt.qt_lib_dir
    print "  -p 3|4  specifically configure for PyQt v3 or v4 [default v4, if found]"
    print "  -r      generate code with tracing enabled [default disabled]"
    print "  -s      QScintilla is a static library and not a DLL (Windows only)"
    print "  -u      build with debugging symbols (requires a debug build of Python on Windows"
    print "  -v dir  where the QScintilla .sip files will be installed [default %s]" % pyqt.pyqt_sip_dir

    sys.exit(rcode)


def inform_user():
    """Tell the user the option values that are going to be used.
    """
    sipconfig.inform("PyQt %s is being used." % pyqt.pyqt_version_str)
    sipconfig.inform("Qt v%s %s edition is being used." % (sipconfig.version_to_string(pyqt.qt_version), pyqt.qt_edition))
    sipconfig.inform("SIP %s is being used." % pyqt.sip_version_str)

    sipconfig.inform("The QScintilla module will be installed in %s." % opt_qscimoddir)
    sipconfig.inform("The QScintilla API file will be installed in %s." % os.path.join(opt_qscidir, "api", "python"))
    sipconfig.inform("The QScintilla .sip files will be installed in %s." % opt_qscisipdir)


def check_qscintilla():
    """See if QScintilla can be found and what its version is.
    """
    # Find the QScintilla header files.
    sciglobal = os.path.join(opt_qsciincdir, "Qsci", "qsciglobal.h")

    if os.access(sciglobal, os.F_OK):
        # Get the QScintilla version string.
        _, sciversstr = sipconfig.read_version(sciglobal, "QScintilla", "QSCINTILLA_VERSION", "QSCINTILLA_VERSION_STR")

        if glob.glob(os.path.join(opt_qscilibdir, "*qscintilla2*")):
            # Because we include the Python bindings with the C++ code we can
            # reasonably force the same version to be used and not bother about
            # versioning.
            if sciversstr != "2.2.1-snapshot-20080309":
                sipconfig.error("QScintilla %s is being used but the Python bindings 2.2.1-snapshot-20080309 are being built.  Please use matching versions." % sciversstr)

            sipconfig.inform("QScintilla %s is being used." % sciversstr)
        else:
            sipconfig.error("The QScintilla library could not be found in %s. If QScintilla is installed then use the -o argument to explicitly specify the correct directory." % opt_qscilibdir)
    else:
        sipconfig.error("Qsci/qsciglobal.h could not be found in %s. If QScintilla is installed then use the -n argument to explicitly specify the correct directory." % opt_qsciincdir)


def sip_flags():
    """Return the SIP flags.
    """
    # Get the flags used for the main PyQt module.
    if pyqt.pyqt_version >= 0x040000:
        flags = pyqt.pyqt_sip_flags.split()
    else:
        flags = pyqt.pyqt_qt_sip_flags.split()
        flags.append("-x")
        flags.append("Qsci_Qt4")

    # Generate the API file.
    flags.append("-a")
    flags.append("QScintilla2.api")

    # Add PyQt's .sip files to the search path.
    flags.append("-I")
    flags.append(pyqt.pyqt_sip_dir)

    return flags


def generate_code():
    """Generate the code for the QScintilla module.
    """
    if pyqt.pyqt_version >= 0x040000:
        mname = "Qsci"
    else:
        mname = "qsci"

    sipconfig.inform("Generating the C++ source for the %s module..." % mname)

    # Build the SIP command line.
    argv = ['"' + pyqt.sip_bin + '"']

    argv.extend(sip_flags())

    if opt_concat:
        argv.append("-j")
        argv.append(str(opt_split))

    if opt_tracing:
        argv.append("-r")

    argv.append("-c")
    argv.append(".")

    buildfile = os.path.join("qsci.sbf")
    argv.append("-b")
    argv.append(buildfile)

    if pyqt.pyqt_version >= 0x040000:
        argv.append("sip/qscimod4.sip")
    else:
        argv.append("sip/qscimod3.sip")

    os.system(" ".join(argv))

    # Check the result.
    if not os.access(buildfile, os.F_OK):
        sipconfig.error("Unable to create the C++ code.")

    # Generate the Makefile.
    sipconfig.inform("Creating the Makefile for the %s module..." % mname)

    def fix_install(mfile):
        if sys.platform != "darwin" or opt_static:
            return

        mfile.write("\tinstall_name_tool -change libqscintilla2.%u.dylib %s/libqscintilla2.%u.dylib $(DESTDIR)%s/$(TARGET)\n" % (QSCI_API_MAJOR, opt_qscilibdir, QSCI_API_MAJOR, opt_qscimoddir))

    if pyqt.pyqt_version >= 0x040000:
        class Makefile(pyqt4.QtGuiModuleMakefile):
            def generate_target_install(self, mfile):
                pyqt4.QtGuiModuleMakefile.generate_target_install(self, mfile)
                fix_install(mfile)
    else:
        class Makefile(pyqt3.QtModuleMakefile):
            def generate_target_install(self, mfile):
                pyqt3.QtModuleMakefile.generate_target_install(self, mfile)
                fix_install(mfile)

    installs = []
    sipfiles = []

    for s in glob.glob("sip/*.sip"):
        sipfiles.append(os.path.join("sip", os.path.basename(s)))

    installs.append([sipfiles, os.path.join(opt_qscisipdir, mname)])

    installs.append(("QScintilla2.api", os.path.join(opt_qscidir, "api", "python")))

    # PyQt v4.2 and later can handle MacOS/X universal binaries.
    if pyqt.pyqt_version >= 0x040200:
        makefile = Makefile(
            configuration=pyqt,
            build_file="qsci.sbf",
            install_dir=opt_qscimoddir,
            installs=installs,
            static=opt_static,
            debug=opt_debug,
            universal=pyqt.universal
        )
    else:
        makefile = Makefile(
            configuration=pyqt,
            build_file="qsci.sbf",
            install_dir=opt_qscimoddir,
            installs=installs,
            static=opt_static,
            debug=opt_debug
        )

    makefile.extra_include_dirs.append(opt_qsciincdir)
    makefile.extra_lib_dirs.append(opt_qscilibdir)
    makefile.extra_libs.append("qscintilla2")

    makefile.generate()


def main(argv):
    """Create the configuration module module.

    argv is the list of command line arguments.
    """
    global pyqt

    # Check SIP is new enough.
    if "snapshot" not in pyqt.sip_version_str:
        if pyqt.sip_version < sip_min_version:
            sipconfig.error("This version of QScintilla requires SIP v%s or later" % sipconfig.version_to_string(sip_min_version))

    # Parse the command line.
    try:
        optlist, args = getopt.getopt(argv[1:], "a:hcd:j:kn:o:p:rsuv:")
    except getopt.GetoptError:
        usage()

    if args:
        usage()

    global opt_qscimoddir, opt_qscisipdir, opt_qsciincdir, opt_qscilibdir
    global opt_concat, opt_split, opt_static, opt_tracing, opt_debug
    global qsci_define, qt_data_dir, opt_qscidir

    for opt, arg in optlist:
        if opt == "-h":
            usage(0)
        elif opt == "-a":
            opt_qscidir = os.path.abspath(arg)
        elif opt == "-c":
            opt_concat = True
        elif opt == "-d":
            opt_qscimoddir = os.path.abspath(arg)
        elif opt == "-j":
            try:
                opt_split = int(arg)
            except:
                usage()
        elif opt == "-k":
            opt_static = True
        elif opt == "-n":
            opt_qsciincdir = os.path.abspath(arg)
        elif opt == "-o":
            opt_qscilibdir = os.path.abspath(arg)
        elif opt == "-p":
            if arg == "4":
                if pyqt4 is None:
                    sipconfig.error("PyQt v4 was specified with the -p argument but doesn't seem to be installed.")
                else:
                    pyqt = pyqt4.Configuration()
                    qt_data_dir = pyqt.qt_data_dir
            elif arg == "3":
                if pyqt3 is None:
                    sipconfig.error("PyQt v3 was specified with the -p argument but doesn't seem to be installed.")
                else:
                    pyqt = pyqt3.Configuration()
                    qt_data_dir = pyqt.qt_dir
            else:
                usage()
        elif opt == "-r":
            opt_tracing = True
        elif opt == "-s":
            qsci_define = ""
        elif opt == "-u":
            opt_debug = True
        elif opt == "-v":
            opt_qscisipdir = os.path.abspath(arg)
        else:
            usage()

    # Now we know which version of PyQt to use we can set defaults for those
    # arguments that weren't specified.
    if opt_qscimoddir is None:
        opt_qscimoddir = pyqt.pyqt_mod_dir

    if opt_qsciincdir is None:
        opt_qsciincdir = pyqt.qt_inc_dir

    if opt_qscilibdir is None:
        opt_qscilibdir = pyqt.qt_lib_dir

    if opt_qscisipdir is None:
        opt_qscisipdir = pyqt.pyqt_sip_dir

    if opt_qscidir is None:
        opt_qscidir = os.path.join(qt_data_dir, "qsci")

    # Check for QScintilla.
    check_qscintilla()

    # Tell the user what's been found.
    inform_user()

    # Generate the code.
    generate_code()


###############################################################################
# The script starts here.
###############################################################################

if __name__ == "__main__":
    try:
        main(sys.argv)
    except SystemExit:
        raise
    except:
        print \
"""An internal error occured.  Please report all the output from the program,
including the following traceback, to support@riverbankcomputing.com.
"""
        raise
