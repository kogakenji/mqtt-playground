TODO

- Docker instalacao mosquitto
- Docker instalacao mongodb
- Docker instalacao grafana (ou outro visualizador de dados)

Padronizar metodo de recuperacao dos dados do sensor para inserir via mosquitto no mongodb.
Possibilidade de insercao dos dados em outras bases de dados (BD, arquivo, etc, via mosquitto)




A CA file has been bootstrapped using certificates from the SystemRoots
keychain. To add additional certificates (e.g. the certificates added in
the System keychain), place .pem files in
  /usr/local/etc/openssl/certs

and run
  /usr/local/opt/openssl/bin/c_rehash

openssl is keg-only, which means it was not symlinked into /usr/local,
because Apple has deprecated use of OpenSSL in favor of its own TLS and crypto libraries.

If you need to have openssl first in your PATH run:
  echo 'export PATH="/usr/local/opt/openssl/bin:$PATH"' >> ~/.zshrc

For compilers to find openssl you may need to set:
  export LDFLAGS="-L/usr/local/opt/openssl/lib"
  export CPPFLAGS="-I/usr/local/opt/openssl/include"

For pkg-config to find openssl you may need to set:
  export PKG_CONFIG_PATH="/usr/local/opt/openssl/lib/pkgconfig"

==> Summary
🍺  /usr/local/Cellar/openssl/1.0.2s: 1,795 files, 12.0MB
==> Installing mosquitto dependency: libuv
==> Downloading https://homebrew.bintray.com/bottles/libuv-1.30.0.mojave.bottle.tar.gz
==> Downloading from https://akamai.bintray.com/2b/2be7b463e83611319cbe311aa3b0242765a2a0a802b1c18861d657159f8a0329?__gda__=exp=1561728049~hmac=339df7a0ff9e689681a58da5077541a0d8aa1
######################################################################## 100.0%
==> Pouring libuv-1.30.0.mojave.bottle.tar.gz
🍺  /usr/local/Cellar/libuv/1.30.0: 48 files, 2.9MB
==> Installing mosquitto
==> Downloading https://homebrew.bintray.com/bottles/mosquitto-1.6.3.mojave.bottle.tar.gz
==> Downloading from https://akamai.bintray.com/ef/ef415ddf4eca59f687fe4e91c3671beaf8141b085588f4ac00b02b0b88e17ce0?__gda__=exp=1561728052~hmac=cf0388250138cb9de9f895d89bacefb30918b
######################################################################## 100.0%
==> Pouring mosquitto-1.6.3.mojave.bottle.tar.gz
Error: The `brew link` step did not complete successfully
The formula built, but is not symlinked into /usr/local
Could not symlink sbin/mosquitto
/usr/local/sbin is not writable.

You can try again using:
  brew link mosquitto
==> Caveats
mosquitto has been installed with a default configuration file.
You can make changes to the configuration by editing:
    /usr/local/etc/mosquitto/mosquitto.conf

To have launchd start mosquitto now and restart at login:
  brew services start mosquitto
Or, if you don't want/need a background service you can just run:
  mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf
==> Summary
🍺  /usr/local/Cellar/mosquitto/1.6.3: 38 files, 956.8KB
Removing: /Users/ivokoga/Library/Caches/Homebrew/mosquitto--1.6.2.mojave.bottle.tar.gz... (295.0KB)
==> Caveats
==> openssl
A CA file has been bootstrapped using certificates from the SystemRoots
keychain. To add additional certificates (e.g. the certificates added in
the System keychain), place .pem files in
  /usr/local/etc/openssl/certs

and run
  /usr/local/opt/openssl/bin/c_rehash

openssl is keg-only, which means it was not symlinked into /usr/local,
because Apple has deprecated use of OpenSSL in favor of its own TLS and crypto libraries.

If you need to have openssl first in your PATH run:
  echo 'export PATH="/usr/local/opt/openssl/bin:$PATH"' >> ~/.zshrc

For compilers to find openssl you may need to set:
  export LDFLAGS="-L/usr/local/opt/openssl/lib"
  export CPPFLAGS="-I/usr/local/opt/openssl/include"

For pkg-config to find openssl you may need to set:
  export PKG_CONFIG_PATH="/usr/local/opt/openssl/lib/pkgconfig"

==> mosquitto
mosquitto has been installed with a default configuration file.
You can make changes to the configuration by editing:
    /usr/local/etc/mosquitto/mosquitto.conf

To have launchd start mosquitto now and restart at login:
  brew services start mosquitto
Or, if you don't want/need a background service you can just run:
  mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf
  