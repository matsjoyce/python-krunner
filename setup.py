#!/usr/bin/env python3

import pathlib
from distutils.core import setup, Extension

from distutils.command.build_ext import build_ext
import subprocess
import os
import shutil


SETUP_DIR = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(SETUP_DIR, "build")


class my_build_ext(build_ext):
    def build_extension(self, ext):
        from_ = os.path.join(BUILD_DIR, "_krunner.so")
        to = self.get_ext_fullpath(ext.name)

        print("Moving", from_, "to", to)

        os.makedirs(os.path.dirname(to), exist_ok=True)
        shutil.copyfile(from_, to)


setup(name="krunner",
      version="0.1",
      author="Matthew Joyce",
      package_dir = {"": os.path.join(SETUP_DIR, "src")},
      py_modules=["krunner"],
      ext_modules=[Extension("_krunner", [])],
      author_email="matsjoyce@gmail.com",
      cmdclass={"build_ext": my_build_ext})
