CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)

PROJECT(six-download NONE)

INCLUDE(ExternalProject)
ExternalProject_Add(six
	URL https://pypi.python.org/packages/16/d8/bc6316cf98419719bd59c91742194c111b6f2e85abac88e496adefaf7afe/six-1.11.0.tar.gz
	URL_HASH SHA256=70e8a77beed4562e7f14fe23a786b54f6296e34344c23bc42f07b15018ff98e9
	SOURCE_DIR "/build_src/pytorch/build/confu-srcs/six"
	BINARY_DIR "/build_src/pytorch/build/confu-deps/six"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	TEST_COMMAND ""
)
