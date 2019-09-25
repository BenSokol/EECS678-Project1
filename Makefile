# @Filename: example-root-Makefile.mk
# @Author:   Ben Sokol
# @Email:    ben@bensokol.com
# @Created:  March 28th, 2019 [1:21pm]
# @Modified: March 28th, 2019 [1:24pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

# Instructions:
# 1. Move this file to project root and rename it to 'Makefile'
# 2. Update to the location of build-tools relative to the root of your project.
# 3. Create a file named 'Makefile-Settings.mk' at project root, and customize as needed.

MAKEFILE_DIR_LOCATION = submodules/build-tools
-include $(MAKEFILE_DIR_LOCATION)/Makefile
