from glob import glob
from setuptools import setup,find_packages
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "_amazons",
        sorted(glob("src/*.cpp")),
    ),
]

setup(packages=find_packages(),ext_modules=ext_modules)
