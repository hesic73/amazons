from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "_amazons",
        sorted(glob("src/*.cpp")),
    ),
]

setup(ext_modules=ext_modules)
