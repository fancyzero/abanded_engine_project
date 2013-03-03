from distutils.core import setup,Extension
include_dirs = ['..\\basis\\src']
libraries = ['basis_Debug_x64']
library_dirs = ['..\\temp\\basis\\x64\\Debug']
extra_link_args = []

mania_basis_module = Extension('_basis',
        sources = ['basis_wrap.cxx'],
        include_dirs = include_dirs,
        libraries = libraries,
        library_dirs = library_dirs,
        extra_link_args = extra_link_args,
        )
setup(name='Mania basis wrapper',
      version='1.0',
      #py_modules=["mania_basis"],
      ext_modules=[mania_basis_module],
     )
