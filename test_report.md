# Relatório de Testes do Minishell
Gerado em: Mon Jun 16 16:04:06 UTC 2025


[0;34m=======================================================================[0m
[0;34m  1. Testes de Compilação [0m
[0;34m=======================================================================[0m

## 1. Testes de Compilação

Verificando flags de compilação (-Wall -Wextra -Werror)...
[0;31mFALHOU: Flags -Wall -Wextra -Werror não encontradas na saída de 'make -n'.[0m
- Flags de compilação: <span style='color:red;'>FALHOU</span>
Compilando o minishell...
[0;32mPASSOU: ./minishell foi compilado com sucesso.[0m
- Compilação: <span style='color:green;'>OK</span>
Verificando se o Makefile não revincula...
[0;32mPASSOU: Makefile não revinculou.[0m
- Re-vinculação: <span style='color:green;'>OK</span>

[0;34m=======================================================================[0m
[0;34m  2. Comando Simples e Variáveis Globais [0m
[0;34m=======================================================================[0m

## 2. Comando Simples e Variáveis Globais

### Teste: `/bin/ls`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
Libft42
Makefile
README.md
bash_output.tmp
lol.c"
make_log.tmp
minishell
minishell.c
minishell.h
minishell_tester
minishell_tester-main-2
objs
outfile.txt
readline.supp
srcs
test.sh
test_report.md
utils
valgrind_report.txt
--- Saída do Minishell (stdout)
Libft42
Makefile
README.md
bash_output.tmp
lol.c"
make_log.tmp
minishell
minishell.c
minishell.h
minishell_error.tmp
minishell_output.tmp
minishell_tester
minishell_tester-main-2
objs
outfile.txt
readline.supp
srcs
test.sh
test_report.md
utils
valgrind_report.txt
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: ` `
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
--- Saída do Minishell (stdout)
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: ``
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
--- Saída do Minishell (stdout)
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
**[AVALIAÇÃO MANUAL]** Pergunte ao aluno sobre o uso de variáveis globais e peça para justificar.


[0;34m=======================================================================[0m
[0;34m  3. Testes de Argumentos [0m
[0;34m=======================================================================[0m

## 3. Testes de Argumentos

### Teste: `/bin/ls -l -a`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
total 812
drwxrwxrwx+ 10 codespace root        4096 Jun 16 16:04 .
drwxr-xrwx+  5 codespace root        4096 Jun 13 16:41 ..
drwxrwxrwx+  9 codespace root        4096 Jun 16 13:31 .git
-rwxrwxrwx   1 codespace codespace 165368 Jun 15 23:18 .nfs00000000108300c600000e3f
-rwxrwxrwx   1 codespace codespace 165400 Jun 15 23:18 .nfs00000000108300f700000e3e
drwxrwxrwx+  2 codespace root        4096 Jun 13 16:41 .vscode
drwxrwxrwx+  2 codespace root        4096 Jun 16 16:04 Libft42
-rw-rw-rw-   1 codespace codespace   3578 Jun 15 23:18 Makefile
-rw-rw-rw-   1 codespace root          15 Jun 13 16:41 README.md
-rw-rw-rw-   1 codespace codespace      0 Jun 16 16:04 bash_output.tmp
-rw-rw-rw-   1 codespace codespace      0 Jun 16 13:30 lol.c"
-rw-rw-rw-   1 codespace codespace     36 Jun 16 16:04 make_log.tmp
-rwxrwxrwx   1 codespace codespace 366472 Jun 16 16:04 minishell
-rw-rw-rw-   1 codespace codespace   2565 Jun 16 05:17 minishell.c
-rw-rw-rw-   1 codespace codespace  28739 Jun 16 03:35 minishell.h
-rw-rw-rw-   1 codespace codespace      0 Jun 16 16:04 minishell_error.tmp
-rw-rw-rw-   1 codespace codespace      5 Jun 16 16:04 minishell_output.tmp
drwxrwxrwx+  2 codespace root        4096 Jun 13 16:41 minishell_tester
drwxrwxrwx+  5 codespace root        4096 Jun 16 16:01 minishell_tester-main-2
drwxrwxrwx+  4 codespace codespace   4096 Jun 16 16:04 objs
-rw-rw-rw-   1 codespace codespace    172 Jun 15 23:18 outfile.txt
-rw-rw-rw-   1 codespace root        1471 Jun 13 17:12 readline.supp
drwxrwxrwx+ 15 codespace root        4096 Jun 13 16:41 srcs
-rwxrwxrwx   1 codespace root       12590 Jun 16 16:03 test.sh
-rw-rw-rw-   1 codespace codespace   2715 Jun 16 16:04 test_report.md
drwxrwxrwx+  2 codespace root        4096 Jun 15 23:18 utils
-rw-r--r--   1 codespace codespace   1127 Jun 13 17:14 valgrind_report.txt
--- Saída do Minishell (stdout)
total 812
drwxrwxrwx+ 10 codespace root        4096 Jun 16 16:04 .
drwxr-xrwx+  5 codespace root        4096 Jun 13 16:41 ..
drwxrwxrwx+  9 codespace root        4096 Jun 16 13:31 .git
-rwxrwxrwx   1 codespace codespace 165368 Jun 15 23:18 .nfs00000000108300c600000e3f
-rwxrwxrwx   1 codespace codespace 165400 Jun 15 23:18 .nfs00000000108300f700000e3e
drwxrwxrwx+  2 codespace root        4096 Jun 13 16:41 .vscode
drwxrwxrwx+  2 codespace root        4096 Jun 16 16:04 Libft42
-rw-rw-rw-   1 codespace codespace   3578 Jun 15 23:18 Makefile
-rw-rw-rw-   1 codespace root          15 Jun 13 16:41 README.md
-rw-rw-rw-   1 codespace codespace   1835 Jun 16 16:04 bash_output.tmp
-rw-rw-rw-   1 codespace codespace      0 Jun 16 13:30 lol.c"
-rw-rw-rw-   1 codespace codespace     36 Jun 16 16:04 make_log.tmp
-rwxrwxrwx   1 codespace codespace 366472 Jun 16 16:04 minishell
-rw-rw-rw-   1 codespace codespace   2565 Jun 16 05:17 minishell.c
-rw-rw-rw-   1 codespace codespace  28739 Jun 16 03:35 minishell.h
-rw-rw-rw-   1 codespace codespace      0 Jun 16 16:04 minishell_error.tmp
-rw-rw-rw-   1 codespace codespace      0 Jun 16 16:04 minishell_output.tmp
drwxrwxrwx+  2 codespace root        4096 Jun 13 16:41 minishell_tester
drwxrwxrwx+  5 codespace root        4096 Jun 16 16:01 minishell_tester-main-2
drwxrwxrwx+  4 codespace codespace   4096 Jun 16 16:04 objs
-rw-rw-rw-   1 codespace codespace    172 Jun 15 23:18 outfile.txt
-rw-rw-rw-   1 codespace root        1471 Jun 13 17:12 readline.supp
drwxrwxrwx+ 15 codespace root        4096 Jun 13 16:41 srcs
-rwxrwxrwx   1 codespace root       12590 Jun 16 16:03 test.sh
-rw-rw-rw-   1 codespace codespace   2715 Jun 16 16:04 test_report.md
drwxrwxrwx+  2 codespace root        4096 Jun 15 23:18 utils
-rw-r--r--   1 codespace codespace   1127 Jun 13 17:14 valgrind_report.txt
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---

[0;34m=======================================================================[0m
[0;34m  4. Testes de Built-ins (echo, pwd, cd, env) [0m
[0;34m=======================================================================[0m

## 4. Testes de Built-ins (echo, pwd, cd, env)

### Teste: `echo hello world`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
hello world
--- Saída do Minishell (stdout)
hello world
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `echo -n hello`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
hello--- Saída do Minishell (stdout)
helloexit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `pwd`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
/workspaces/novo_reset
--- Saída do Minishell (stdout)
/workspaces/novo_reset
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `env`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
SHELL=/bin/bash
NUGET_XMLDOC_MODE=skip
COLORTERM=truecolor
CLOUDENV_ENVIRONMENT_ID=d706a6db-8822-40d5-8b01-efd2b944615a
NVM_INC=/usr/local/share/nvm/versions/node/v20.19.0/include/node
TERM_PROGRAM_VERSION=1.101.0
GITHUB_USER=YuriVdSantos
rvm_prefix=/usr/local
CODESPACE_NAME=fluffy-giggle-vw699r4p6p9cxx94
HOSTNAME=codespaces-a73d6b
JAVA_ROOT=/home/codespace/java
JAVA_HOME=/usr/local/sdkman/candidates/java/current
DOTNET_ROOT=/usr/share/dotnet
CODESPACES=true
PYTHON_ROOT=/home/codespace/.python
GRADLE_HOME=/usr/local/sdkman/candidates/gradle/current
NVS_DIR=/usr/local/nvs
NVS_OS=linux
DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1
MY_RUBY_HOME=/usr/local/rvm/rubies/ruby-3.3.4
NVS_USE_XZ=1
SDKMAN_CANDIDATES_DIR=/usr/local/sdkman/candidates
RUBY_VERSION=ruby-3.3.4
PWD=/workspaces/novo_reset
PIPX_BIN_DIR=/usr/local/py-utils/bin
rvm_version=1.29.12 (latest)
ORYX_DIR=/usr/local/oryx
_=/usr/bin/env
ContainerVersion=13
VSCODE_GIT_ASKPASS_NODE=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/node
HUGO_ROOT=/home/codespace/.hugo
GITHUB_CODESPACES_PORT_FORWARDING_DOMAIN=app.github.dev
NPM_GLOBAL=/home/codespace/.npm-global
HOME=/home/codespace
GITHUB_API_URL=https://api.github.com
LANG=C.UTF-8
GITHUB_TOKEN=ghu_SAk7Ua5gJ4LaMWmH2VMHbe8STwSXof0IqYPl
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
DYNAMIC_INSTALL_ROOT_DIR=/opt
NVM_SYMLINK_CURRENT=true
PHP_PATH=/usr/local/php/current
DEBIAN_FLAVOR=focal-scm
GIT_ASKPASS=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/extensions/git/dist/askpass.sh
PHP_ROOT=/home/codespace/.php
ORYX_ENV_TYPE=vsonline-present
HUGO_DIR=/usr/local/hugo/bin
DOCKER_BUILDKIT=1
GOROOT=/usr/local/go
INTERNAL_VSCS_TARGET_URL=https://eastus2.online.visualstudio.com
SHELL_LOGGED_IN=true
PYTHON_PATH=/usr/local/python/current
NVM_DIR=/usr/local/share/nvm
VSCODE_GIT_ASKPASS_EXTRA_ARGS=
rvm_bin_path=/usr/local/rvm/bin
GEM_PATH=/usr/local/rvm/gems/ruby-3.3.4:/usr/local/rvm/gems/ruby-3.3.4@global
GEM_HOME=/usr/local/rvm/gems/ruby-3.3.4
GITHUB_CODESPACE_TOKEN=APQ2TDZZXQP4Y7E3GT3AJQDIKDQM5ANCNFSM4AOFH64A
LESSCLOSE=/usr/bin/lesspipe %s %s
NVS_ROOT=/usr/local/nvs
GITHUB_GRAPHQL_URL=https://api.github.com/graphql
TERM=xterm-256color
LESSOPEN=| /usr/bin/lesspipe %s
USER=codespace
NODE_ROOT=/home/codespace/nvm
VSCODE_GIT_IPC_HANDLE=/tmp/vscode-git-17d6610aa7.sock
PYTHONIOENCODING=UTF-8
GITHUB_SERVER_URL=https://github.com
NVS_HOME=/usr/local/nvs
PIPX_HOME=/usr/local/py-utils
CONDA_SCRIPT=/opt/conda/etc/profile.d/conda.sh
MAVEN_HOME=/usr/local/sdkman/candidates/maven/current
SDKMAN_DIR=/usr/local/sdkman
SHLVL=3
NVM_CD_FLAGS=
ORYX_SDK_STORAGE_BASE_URL=https://oryx-cdn.microsoft.io
GIT_EDITOR=code --wait
CONDA_DIR=/opt/conda
PROMPT_DIRTRIM=4
SDKMAN_CANDIDATES_API=https://api.sdkman.io/2
DOTNET_RUNNING_IN_CONTAINER=true
DOTNET_USE_POLLING_FILE_WATCHER=true
ENABLE_DYNAMIC_INSTALL=true
MAVEN_ROOT=/home/codespace/.maven
ORYX_PREFER_USER_INSTALLED_SDKS=true
JUPYTERLAB_PATH=/home/codespace/.local/bin
RVM_PATH=/usr/local/rvm
GITHUB_REPOSITORY=YuriVdSantos/novo_reset
RAILS_DEVELOPMENT_HOSTS=.githubpreview.dev,.preview.app.github.dev,.app.github.dev
VSCODE_GIT_ASKPASS_MAIN=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/extensions/git/dist/askpass-main.js
RUBY_ROOT=/home/codespace/.ruby
RUBY_HOME=/usr/local/rvm/rubies/default
BROWSER=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/bin/helpers/browser.sh
PATH=/usr/local/rvm/gems/ruby-3.3.4/bin:/usr/local/rvm/gems/ruby-3.3.4@global/bin:/usr/local/rvm/rubies/ruby-3.3.4/bin:/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/bin/remote-cli:/home/codespace/.local/bin:/home/codespace/.dotnet:/home/codespace/nvm/current/bin:/home/codespace/.php/current/bin:/home/codespace/.python/current/bin:/home/codespace/java/current/bin:/home/codespace/.ruby/current/bin:/home/codespace/.local/bin:/usr/local/python/current/bin:/usr/local/py-utils/bin:/usr/local/jupyter:/usr/local/oryx:/usr/local/go/bin:/go/bin:/usr/local/sdkman/bin:/usr/local/sdkman/candidates/java/current/bin:/usr/local/sdkman/candidates/gradle/current/bin:/usr/local/sdkman/candidates/maven/current/bin:/usr/local/sdkman/candidates/ant/current/bin:/usr/local/rvm/gems/default/bin:/usr/local/rvm/gems/default@global/bin:/usr/local/rvm/rubies/default/bin:/usr/local/share/rbenv/bin:/usr/local/php/current/bin:/opt/conda/bin:/usr/local/nvs:/usr/local/share/nvm/versions/node/v20.19.0/bin:/usr/local/hugo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/share/dotnet:/home/codespace/.dotnet/tools:/usr/local/rvm/bin
CODESPACE_VSCODE_FOLDER=/workspaces/novo_reset
SDKMAN_PLATFORM=linuxx64
NVM_BIN=/usr/local/share/nvm/versions/node/v20.19.0/bin
IRBRC=/usr/local/rvm/rubies/ruby-3.3.4/.irbrc
rvm_path=/usr/local/rvm
OLDPWD=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1
GOPATH=/go
TERM_PROGRAM=vscode
VSCODE_IPC_HOOK_CLI=/tmp/vscode-ipc-1085f2da-9c76-4d6d-8ed0-e10a0f69befe.sock
--- Saída do Minishell (stdout)
SHELL=/bin/bash
NUGET_XMLDOC_MODE=skip
COLORTERM=truecolor
CLOUDENV_ENVIRONMENT_ID=d706a6db-8822-40d5-8b01-efd2b944615a
NVM_INC=/usr/local/share/nvm/versions/node/v20.19.0/include/node
TERM_PROGRAM_VERSION=1.101.0
GITHUB_USER=YuriVdSantos
rvm_prefix=/usr/local
CODESPACE_NAME=fluffy-giggle-vw699r4p6p9cxx94
HOSTNAME=codespaces-a73d6b
JAVA_ROOT=/home/codespace/java
JAVA_HOME=/usr/local/sdkman/candidates/java/current
DOTNET_ROOT=/usr/share/dotnet
CODESPACES=true
GRADLE_HOME=/usr/local/sdkman/candidates/gradle/current
PYTHON_ROOT=/home/codespace/.python
NVS_DIR=/usr/local/nvs
NVS_OS=linux
DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1
MY_RUBY_HOME=/usr/local/rvm/rubies/ruby-3.3.4
NVS_USE_XZ=1
SDKMAN_CANDIDATES_DIR=/usr/local/sdkman/candidates
RUBY_VERSION=ruby-3.3.4
PIPX_BIN_DIR=/usr/local/py-utils/bin
PWD=/workspaces/novo_reset
rvm_version=1.29.12 (latest)
ORYX_DIR=/usr/local/oryx
ContainerVersion=13
VSCODE_GIT_ASKPASS_NODE=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/node
HUGO_ROOT=/home/codespace/.hugo
GITHUB_CODESPACES_PORT_FORWARDING_DOMAIN=app.github.dev
NPM_GLOBAL=/home/codespace/.npm-global
HOME=/home/codespace
GITHUB_API_URL=https://api.github.com
LANG=C.UTF-8
GITHUB_TOKEN=ghu_SAk7Ua5gJ4LaMWmH2VMHbe8STwSXof0IqYPl
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
DYNAMIC_INSTALL_ROOT_DIR=/opt
NVM_SYMLINK_CURRENT=true
PHP_PATH=/usr/local/php/current
DEBIAN_FLAVOR=focal-scm
GIT_ASKPASS=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/extensions/git/dist/askpass.sh
PHP_ROOT=/home/codespace/.php
ORYX_ENV_TYPE=vsonline-present
HUGO_DIR=/usr/local/hugo/bin
DOCKER_BUILDKIT=1
GOROOT=/usr/local/go
INTERNAL_VSCS_TARGET_URL=https://eastus2.online.visualstudio.com
SHELL_LOGGED_IN=true
PYTHON_PATH=/usr/local/python/current
NVM_DIR=/usr/local/share/nvm
VSCODE_GIT_ASKPASS_EXTRA_ARGS=
rvm_bin_path=/usr/local/rvm/bin
GEM_PATH=/usr/local/rvm/gems/ruby-3.3.4:/usr/local/rvm/gems/ruby-3.3.4@global
GEM_HOME=/usr/local/rvm/gems/ruby-3.3.4
GITHUB_CODESPACE_TOKEN=APQ2TDZZXQP4Y7E3GT3AJQDIKDQM5ANCNFSM4AOFH64A
LESSCLOSE=/usr/bin/lesspipe %s %s
NVS_ROOT=/usr/local/nvs
GITHUB_GRAPHQL_URL=https://api.github.com/graphql
TERM=xterm-256color
LESSOPEN=| /usr/bin/lesspipe %s
USER=codespace
NODE_ROOT=/home/codespace/nvm
VSCODE_GIT_IPC_HANDLE=/tmp/vscode-git-17d6610aa7.sock
PYTHONIOENCODING=UTF-8
GITHUB_SERVER_URL=https://github.com
PIPX_HOME=/usr/local/py-utils
NVS_HOME=/usr/local/nvs
CONDA_SCRIPT=/opt/conda/etc/profile.d/conda.sh
MAVEN_HOME=/usr/local/sdkman/candidates/maven/current
SDKMAN_DIR=/usr/local/sdkman
SHLVL=2
NVM_CD_FLAGS=
ORYX_SDK_STORAGE_BASE_URL=https://oryx-cdn.microsoft.io
GIT_EDITOR=code --wait
CONDA_DIR=/opt/conda
PROMPT_DIRTRIM=4
DOTNET_RUNNING_IN_CONTAINER=true
SDKMAN_CANDIDATES_API=https://api.sdkman.io/2
DOTNET_USE_POLLING_FILE_WATCHER=true
ENABLE_DYNAMIC_INSTALL=true
MAVEN_ROOT=/home/codespace/.maven
ORYX_PREFER_USER_INSTALLED_SDKS=true
JUPYTERLAB_PATH=/home/codespace/.local/bin
RVM_PATH=/usr/local/rvm
GITHUB_REPOSITORY=YuriVdSantos/novo_reset
RAILS_DEVELOPMENT_HOSTS=.githubpreview.dev,.preview.app.github.dev,.app.github.dev
VSCODE_GIT_ASKPASS_MAIN=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/extensions/git/dist/askpass-main.js
RUBY_ROOT=/home/codespace/.ruby
RUBY_HOME=/usr/local/rvm/rubies/default
BROWSER=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/bin/helpers/browser.sh
PATH=/usr/local/rvm/gems/ruby-3.3.4/bin:/usr/local/rvm/gems/ruby-3.3.4@global/bin:/usr/local/rvm/rubies/ruby-3.3.4/bin:/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1/bin/remote-cli:/home/codespace/.local/bin:/home/codespace/.dotnet:/home/codespace/nvm/current/bin:/home/codespace/.php/current/bin:/home/codespace/.python/current/bin:/home/codespace/java/current/bin:/home/codespace/.ruby/current/bin:/home/codespace/.local/bin:/usr/local/python/current/bin:/usr/local/py-utils/bin:/usr/local/jupyter:/usr/local/oryx:/usr/local/go/bin:/go/bin:/usr/local/sdkman/bin:/usr/local/sdkman/candidates/java/current/bin:/usr/local/sdkman/candidates/gradle/current/bin:/usr/local/sdkman/candidates/maven/current/bin:/usr/local/sdkman/candidates/ant/current/bin:/usr/local/rvm/gems/default/bin:/usr/local/rvm/gems/default@global/bin:/usr/local/rvm/rubies/default/bin:/usr/local/share/rbenv/bin:/usr/local/php/current/bin:/opt/conda/bin:/usr/local/nvs:/usr/local/share/nvm/versions/node/v20.19.0/bin:/usr/local/hugo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/share/dotnet:/home/codespace/.dotnet/tools:/usr/local/rvm/bin
CODESPACE_VSCODE_FOLDER=/workspaces/novo_reset
SDKMAN_PLATFORM=linuxx64
NVM_BIN=/usr/local/share/nvm/versions/node/v20.19.0/bin
IRBRC=/usr/local/rvm/rubies/ruby-3.3.4/.irbrc
rvm_path=/usr/local/rvm
OLDPWD=/vscode/bin/linux-x64/dfaf44141ea9deb3b4096f7cd6d24e00c147a4b1
GOPATH=/go
TERM_PROGRAM=vscode
VSCODE_IPC_HOOK_CLI=/tmp/vscode-ipc-1085f2da-9c76-4d6d-8ed0-e10a0f69befe.sock
_=./minishell
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
Testando 'cd'...
[0;31mFALHOU[0m (cd e pwd não funcionaram como esperado)
### Teste: `cd`
**Resultado:** <span style='color:red;'>FALHOU</span>
---

[0;34m=======================================================================[0m
[0;34m  5. Teste do 'exit' [0m
[0;34m=======================================================================[0m

## 5. Teste do 'exit'


Testando 'exit 42'...
[0;32mPASSOU: 'exit 42' retornou 42.[0m
### Teste: `exit 42`
**Resultado:** <span style='color:green;'>PASSOU</span>
---

Testando 'exit' (sem argumentos)...
[0;32mPASSOU: 'exit' sem argumentos retornou o status do último comando (2).[0m
### Teste: `exit` sem args
**Resultado:** <span style='color:green;'>PASSOU</span>
---

[0;34m=======================================================================[0m
[0;34m  6. Teste de Valor de Retorno ($?) [0m
[0;34m=======================================================================[0m

## 6. Teste de Valor de Retorno ($?)

### Teste: `ls; echo $?`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
Libft42
Makefile
README.md
bash_output.tmp
lol.c"
make_log.tmp
minishell
minishell.c
minishell.h
minishell_error.tmp
minishell_output.tmp
minishell_tester
minishell_tester-main-2
objs
outfile.txt
readline.supp
srcs
test.sh
test_report.md
utils
valgrind_report.txt
0
--- Saída do Minishell (stdout)
exit
```
#### Diferença no Exit Code:
* **Bash:** `0`
* **Minishell:** `127`
#### Saída de Erro (stderr) do Minishell:
```
command not found: ls;
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `ls non_existent_file; echo $?`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
ls: cannot access 'non_existent_file': No such file or directory
2
--- Saída do Minishell (stdout)
exit
```
#### Diferença no Exit Code:
* **Bash:** `0`
* **Minishell:** `2`
#### Saída de Erro (stderr) do Minishell:
```
ls: cannot access 'non_existent_file;': No such file or directory
ls: cannot access 'echo': No such file or directory
ls: cannot access '0': No such file or directory
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---

[0;34m=======================================================================[0m
[0;34m  7. Testes de Aspas (Simples e Duplas) [0m
[0;34m=======================================================================[0m

## 7. Testes de Aspas (Simples e Duplas)

### Teste: `echo "hello   world"`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
hello   world
--- Saída do Minishell (stdout)
hello   world
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `echo 'hello   world'`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
hello
--- Saída do Minishell (stdout)
hello   world
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `echo 'echo $USER'`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
echo
--- Saída do Minishell (stdout)
echo $USER
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `echo "echo $USER"`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
echo codespace
--- Saída do Minishell (stdout)
echo codespace
exit
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---

[0;34m=======================================================================[0m
[0;34m  8. Testes de 'export' e 'unset' [0m
[0;34m=======================================================================[0m

## 8. Testes de 'export' e 'unset'

### Teste: `export TEST_VAR=minishell && env | grep TEST_VAR`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
TEST_VAR=minishell
--- Saída do Minishell (stdout)
exit
```
#### Diferença no Exit Code:
* **Bash:** `0`
* **Minishell:** `1`
#### Saída de Erro (stderr) do Minishell:
```
minishell: export: `&&': not a valid identifier
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `export TEST_VAR=minishell && export TEST_VAR=rocks && env | grep TEST_VAR`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
TEST_VAR=rocks
--- Saída do Minishell (stdout)
exit
```
#### Diferença no Exit Code:
* **Bash:** `0`
* **Minishell:** `1`
#### Saída de Erro (stderr) do Minishell:
```
minishell: export: `&&': not a valid identifier
minishell: export: `&&': not a valid identifier
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---
### Teste: `export TEST_VAR=minishell && unset TEST_VAR && env | grep TEST_VAR`
**Resultado:** <span style='color:red;'>FALHOU</span>
#### Diferenças na Saída:
```diff
--- Saída do Bash (stdout)
--- Saída do Minishell (stdout)
exit
```
#### Saída de Erro (stderr) do Minishell:
```
minishell: export: `&&': not a valid identifier
minishell: export: `&&': not a valid identifier
```
**[AVALIAÇÃO MANUAL]** Analise a falha documentada no relatório e na consola.

---

[0;34m=======================================================================[0m
[0;34m  9. Testes de Caminhos (Relativo e de Ambiente) [0m
[0;34m=======================================================================[0m

## 9. Testes de Caminhos (Relativo e de Ambiente)

Testando caminho relativo...
### Teste: `./minishell`
