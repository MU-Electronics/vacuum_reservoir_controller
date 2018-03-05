
# Project type includes sub dirs
TEMPLATE = subdirs

# Include the confiruation file
include(config.pri)

# Increase sub projects
SUBDIRS += vendor/fluid vendor/qutipi-cpp src

# Include read me files and licensing
DISTFILES += \
    readme.md
