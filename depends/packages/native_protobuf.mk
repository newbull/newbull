package=native_protobuf
$(package)_version=3.0.2
$(package)_download_path=http://127.0.0.1/20161015/bitcoin_depends
$(package)_file_name=protobuf-cpp-$($(package)_version).tar.gz
$(package)_sha256_hash=0a6f73ab32b2888bf7f8c29608f8624a78950de4ae992c3688c3b123b6c84802

define $(package)_set_vars
$(package)_config_opts=--disable-shared
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE) -C src protoc
endef

define $(package)_stage_cmds
  $(MAKE) -C src DESTDIR=$($(package)_staging_dir) install-strip
endef

define $(package)_postprocess_cmds
  rm -rf lib include
endef
