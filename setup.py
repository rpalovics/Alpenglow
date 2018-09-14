try:
    from setuptools import setup, Extension
except:
    from distutils.core import setup, Extension

import sipdistutils
import pkg_resources
import os
import os.path
import sys
from sys import platform

from distutils.dep_util import newer_group
from distutils.errors import *
from distutils.sysconfig import get_config_vars
import distutils.ccompiler
import re

# recursively adds .sip files to dependencies
class custom_build_ext(sipdistutils.build_ext):
    def _find_sip_extra_depends(self, file):

        dirname = os.path.dirname(file)
        if(dirname == ""):
            dirname = "."
        content = open(file, "rb").read()
        new_sources = [
            dirname + "/" + s.strip()[9:]
            for s in content.decode().splitlines(False)
            if s.strip().startswith('%Include')
        ]

        new_h_sources = []
        h_local_regex=re.compile('^\s*#include\s*"([^"]+?)"')
        new_h_sources = [
            h 
            for s in content.decode().splitlines(False)
            for h in h_local_regex.findall(s)
        ]
        new_depends = set(new_sources) | set(new_h_sources)
        for s in new_sources:
            new_depends = new_depends | self._find_sip_extra_depends(s)

        return new_depends

    def _add_ext_extra_depends(self, ext):
        sources = ext.sources
        if sources is not None and isinstance(sources, (list, tuple)):
            sources = list(sources)
            depends = set()
            for s in sources:
                if s.endswith(".sip"):
                    depends = depends | self._find_sip_extra_depends(s)
            ext.depends += list(depends) + sources

    def build_extension(self, ext):
        self._add_ext_extra_depends(ext)
        ext.include_dirs.append(pkg_resources.resource_filename('numpy', 'core/include'))
        return sipdistutils.build_ext.build_extension(self, ext)

# monkey pach to force distutils to compile in parallel
def nParalellCompile(N):
    def paralellCCompile(self, sources, output_dir=None, macros=None, include_dirs=None, debug=0, extra_preargs=None, extra_postargs=None, depends=None):
        macros, objects, extra_postargs, pp_opts, build = self._setup_compile(output_dir, macros, include_dirs, sources, depends, extra_postargs)
        cc_args = self._get_cc_args(pp_opts, debug, extra_preargs)

        import multiprocessing.pool

        def _single_compile(obj):
            try:
                src, ext = build[obj]
            except KeyError:
                return
            self._compile(obj, src, ext, cc_args, extra_postargs, pp_opts)
        list(multiprocessing.pool.ThreadPool(N).imap(_single_compile, objects))
        return objects
    return paralellCCompile

if os.path.isfile('.parallel'):
    with open('.parallel', 'r') as f:
        n = int(f.readline().strip())
        distutils.ccompiler.CCompiler.compile = nParalellCompile(n)

# function to recursively find cpp sources
def get_cpp_recursively(rdir, ignores=[]):
    def is_ignored(f):
        for p in ignores:
            if f.startswith(p) or f[len(rdir) + 1:].startswith(p):
                return True
        return False

    cpps = []
    for dirName, subdirList, fileList in os.walk(rdir):
        for fname in fileList:
            if fname.endswith(".cpp") and not is_ignored(dirName + "/" + fname):
                cpps.append(dirName + "/" + fname)
    return cpps

# finding cpp sources
cpp_sources = get_cpp_recursively(
    "cpp/src/main",
)

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")
        cfg_vars[key] = value.replace("-g", "")
        cfg_vars[key] = value.replace("-O3", "")

platform_specific_flags = []
if platform == "linux" or platform == "linux2":
    platform_specific_flags = [
        '-mfpmath=sse,387',
        '-Wno-deprecated',
        '-Wno-reorder',
        '-msse2'
        # for modern processors:
        # '-mfma',
        # if you want to eigen to use blas/lapack:
        # '-DEIGEN_USE_BLAS',
        # '-DEIGEN_USE_LAPACKE',
    ]
elif platform == "darwin":
    platform_specific_flags = [
        '-stdlib=libc++',
        '-Wno-deprecated',
        '-Wno-reorder',
    ]
elif platform == "win32":
    platform_specific_flags = [
        '-Dand=&&',
        '-Dor=||',
        '-Duint="unsigned int"',
        '-DMEMORY_USAGE_LOGGER',
        '-O2',
        '-fp=strict'
    ]

conda_executable_name = sys.executable
conda_include_dirs = []
if conda_executable_name[-len("bin/python"):] == "bin/python":
    conda_include_dirs.append(conda_executable_name[:-len("bin/python")]+"include")
elif conda_executable_name[-len("python.exe"):] == "python.exe":
    conda_include_dirs.append(conda_executable_name[:-len("python.exe")]+"Library/include")

setup(
    name='alpenglow',
    version='0.1.0',
    install_requires=['numpy', 'pandas'],
    setup_requires=['numpy'],
    ext_modules=[
        Extension(
            "alpenglow.cpp",
            sources=[
                "sip/recsys.sip",
            ] + cpp_sources,
            include_dirs=[
                '.',
                'cpp/src',
                'cpp/src/main',
                'cpp/dep/gtest/include',
            ]+conda_include_dirs,
            extra_compile_args=[
                '-std=c++11',
                '-O2',
                '-funroll-loops',
                '-fomit-frame-pointer',
                '-lpthread',
                '-msse3',
                '-D_LARGEFILE_SOURCE',
                '-D_FILE_OFFSET_BITS=64'
            ] + platform_specific_flags,
            libraries=[
                # if you want to eigen to use blas/lapack:
                # 'lapack',
                # 'blas',
            ]
        ),
    ],
    packages=[
        'alpenglow',
        'alpenglow.experiments',
        'alpenglow.evaluation',
        'alpenglow.offline',
        'alpenglow.offline.models',
        'alpenglow.offline.evaluation',
        'alpenglow.utils',
    ],
    package_dir={
        'alpenglow': 'python/alpenglow',
    },
    cmdclass={
        'build_ext': custom_build_ext
    }
)
