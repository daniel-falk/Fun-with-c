import os
from setuptools import setup, Extension

module = Extension('spam', sources=['simple_mod.c'])

setup(name='spam', ext_modules = [module])
