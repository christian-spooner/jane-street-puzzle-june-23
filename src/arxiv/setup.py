import numpy
from Cython.Build import cythonize
from setuptools import Extension, setup

extensions = [
    Extension(
        "cutils",
        sources=["cutils.pyx", "gridvar.cpp"],
        include_dirs=[numpy.get_include()],
        language="c++",
    ),
]

setup(
    name="cutils",
    ext_modules=cythonize(extensions),
)
