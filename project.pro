
# Project type includes sub dirs
TEMPLATE = subdirs

# Increase sub projects
SUBDIRS += vendor/fluid vendor/qutipi-cpp src

CONFIG += ordered

# Set build order
vendor/qutipi-cpp.depends = vendor/fluid
src.depends = vendor/fluid
src.depends = vendor/qutipi-cpp

# Include read me files and licensing
DISTFILES += \
    readme.md
