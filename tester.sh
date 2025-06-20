#!/bin/bash

# ==============================================================================
#                     Script de Testes para o Minishell (42)
# ==============================================================================
# Este script foi projetado para testar as funcionalidades do projeto Minishell,
# comparando o comportamento do seu executável com o do bash.
#
# Como usar:
# 1. Coloque este ficheiro (tester.sh) na raiz do seu projeto Minishell.
# 2. Certifique-se de que o seu executável se chama `minishell`.
# 3. Dê permissão de execução ao script: chmod +x tester.sh
# 4. Execute o script: ./tester.sh
# ==============================================================================

# --- Cores para o output ---
GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

# --- Variáveis ---
MINISHELL_PATH="./minishell"
BASH_OUT="bash_out.log"
MINISHELL_OUT="minishell_out.log"
BASH_ERR="bash_err.log"
MINISHELL_ERR="minishell_err.log"
TEST_COUNT=0
PASS_COUNT=0

# --- Funções Auxiliares ---

# Imprime o cabeçalho de uma secção de testes
print_header() {
    echo -e "\n${BLUE}=====================================================${RESET}"
    echo -e "${BLUE}  $1"
    echo -e "${BLUE}=====================================================${RESET}"
}

# Função principal de teste
# Compara a saída (stdout e stderr) e o código de saída do minishell e do bash
run_test() {
    local cmd="$1"
    local description="$2"
    TEST_COUNT=$((TEST_COUNT + 1))

    echo -ne "${YELLOW}Testando: ${description}${RESET} - "
    echo -e "${YELLOW}Comando: [${cmd}]${RESET}"

    # Executa no Bash
    echo -n "$cmd" | bash > "$BASH_OUT" 2> "$BASH_ERR"
    bash_status=$?

    # Executa no Minishell
    echo -n "$cmd" | "$MINISHELL_PATH" > "$MINISHELL_OUT" 2> "$MINISHELL_ERR"
    minishell_status=$?

    # Compara stdout
    diff "$BASH_OUT" "$MINISHELL_OUT" > /dev/null
    stdout_diff=$?

    # Compara stderr (ignorando a mensagem do prompt do minishell, se houver)
    # Bash costuma imprimir o nome do programa no erro, ex: "bash: ls: ...". Normalizamos isso.
    sed 's/bash: //' "$BASH_ERR" > bash_err_normalized.log
    sed 's/minishell: //' "$MINISHELL_ERR" > minishell_err_normalized.log
    diff bash_err_normalized.log minishell_err_normalized.log > /dev/null
    stderr_diff=$?

    # Normalizar códigos de saída (alguns códigos > 128 são por sinais, podem variar)
    if [ $bash_status -gt 128 ]; then bash_status=126; fi; # Command cannot execute
    if [ $minishell_status -gt 128 ]; then minishell_status=126; fi;

    # Verifica o resultado
    if [ $stdout_diff -eq 0 ] && [ $stderr_diff -eq 0 ] && [ $bash_status -eq $minishell_status ]; then
        echo -e "${GREEN}[OK]${RESET}"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}[KO]${RESET}"
        echo "--- BASH STDOUT ---"
        cat "$BASH_OUT"
        echo "--- MINISHELL STDOUT ---"
        cat "$MINISHELL_OUT"
        echo "--- BASH STDERR ---"
        cat "$BASH_ERR"
        echo "--- MINISHELL STDERR ---"
        cat "$MINISHELL_ERR"
        echo "--- STATUS ---"
        echo "BASH exit: $bash_status | MINISHELL exit: $minishell_status"
        echo "-------------------"
    fi
}

# --- Início dos Testes ---

# Compila o projeto
print_header "Compilando o projeto"
make
if [ ! -f "$MINISHELL_PATH" ]; then
    echo -e "${RED}Erro: O executável 'minishell' não foi encontrado. Abortando.${RESET}"
    exit 1
fi
echo -e "${GREEN}Compilação bem-sucedida.${RESET}"


# Preparação de ficheiros de teste
echo "Ficheiro de teste para redirecionamentos." > test_file.txt
echo "Linha para append." >> test_file.txt

# --- Testes Básicos ---
print_header "Testes Básicos e Argumentos"
run_test " /bin/ls " "Comando simples com caminho absoluto"
run_test "/bin/ls -l -a" "Comando com argumentos"
run_test "    ls    -l    " "Comando com múltiplos espaços"
run_test "" "Comando vazio (apenas Enter)"
run_test "    " "Apenas espaços"

# --- Echo ---
print_header "Testes do `echo`"
run_test "echo" "echo sem argumentos"
run_test "echo hello world" "echo com múltiplos argumentos"
run_test "echo -n hello" "echo com a flag -n"
run_test "echo -n -n -n hello world" "echo com múltiplas flags -n"
run_test "echo hello -n world" "echo com -n no meio"

# --- Aspas (Quotes) ---
print_header "Testes de Aspas Simples e Duplas"
run_test "echo \"hello world\"" "Aspas duplas simples"
run_test "echo 'hello world'" "Aspas simples"
run_test "echo \"'hello'\"" "Aspas simples dentro de duplas"
run_test "echo '\"world\"'" "Aspas duplas dentro de simples"
run_test "echo \"hello      world\"" "Espaços múltiplos dentro de aspas duplas"
run_test "echo 'hello      world'" "Espaços múltiplos dentro de aspas simples"

# --- Variáveis de Ambiente ---
print_header "Testes de Variáveis de Ambiente (\$)"
run_test "echo \$USER" "Expansão de variável simples"
run_test "echo \"Hello, \$USER\"" "Expansão dentro de aspas duplas"
run_test "echo 'Hello, \$USER'" "Não expansão dentro de aspas simples"
run_test "echo \$NONEXISTENT_VAR" "Variável inexistente"
run_test "echo \$?" "Código de saída do último comando (ls)"
run_test "ls non_existent_file_123; echo \$?" "Código de saída de comando falho"
run_test "echo \$USER\$SHELL" "Múltiplas variáveis juntas"

# --- Redirecionamentos ---
print_header "Testes de Redirecionamentos (>, >>, <, <<)"
run_test "ls > out.log" "Redirecionamento de saída (>)"
run_test "cat < out.log" "Redirecionamento de entrada (<)"
run_test "echo \" appended\" >> out.log" "Redirecionamento de append (>>)"
run_test "cat < out.log" "Verificar append"
run_test "< test_file.txt cat" "Redirecionamento de entrada no início"
# Teste de Here-document (<<)
run_test "cat << EOF
hello
world
EOF" "Here-document básico"
run_test "cat << \"EOF\"
hello \$USER
EOF" "Here-document com delimitador entre aspas (sem expansão)"
run_test "> out1 > out2 ls" "Múltiplos redirects de saída"
run_test "< test_file.txt < out.log cat" "Múltiplos redirects de entrada"

# --- Pipes ---
print_header "Testes de Pipes (|)"
run_test "ls -l | grep .c | wc -l" "Pipeline com 3 comandos"
run_test "cat < test_file.txt | grep 'teste'" "Pipeline com redirect"
run_test "comando_invalido | wc" "Pipeline com comando inválido"
run_test "cat | ls" "Pipe com comando que espera stdin (ignorado pelo runner, mas testa a estrutura)"

# --- Built-ins ---
print_header "Testes de Built-ins"
run_test "pwd" "pwd"
run_test "env" "env"
run_test "export TEST_VAR=123; env | grep TEST_VAR" "export e verificação com env"
run_test "export TEST_VAR=456; env | grep TEST_VAR" "export para atualizar variável"
run_test "export NO_VALUE_VAR; env | grep NO_VALUE_VAR" "export sem valor"
run_test "unset TEST_VAR; env | grep TEST_VAR" "unset de variável"
# Testes de `cd` não podem ser automatizados desta forma, pois executam num subprocesso.
# Devem ser verificados manualmente.
echo -e "${YELLOW}AVISO: Testes para 'cd' e 'exit' devem ser feitos manualmente.${RESET}"
echo "Exemplos manuais:"
echo "  cd .."
echo "  pwd"
echo "  cd /tmp"
echo "  pwd"
echo "  exit 42"

# --- Erros de Sintaxe e Casos Complexos ---
print_header "Testes de Erros e Casos Complexos"
run_test "asdasd qweqwe" "Comando inválido"
run_test "echo <" "Erro de sintaxe (redirect sem ficheiro)"
run_test "|" "Erro de sintaxe (pipe no início)"
run_test "echo hello >" "Erro de sintaxe (redirect no fim)"
run_test "cat < non_existent_file_123" "Redirect de entrada com ficheiro inexistente"
run_test "unset PATH; ls" "Comando sem PATH"
run_test "export PATH=/bin:/usr/bin; ls" "Restaurar PATH e executar"

# --- Relatório Final ---
print_header "Relatório Final"
echo -e "Total de testes: ${BLUE}${TEST_COUNT}${RESET}"
echo -e "Testes aprovados: ${GREEN}${PASS_COUNT}${RESET}"
FAIL_COUNT=$((TEST_COUNT - PASS_COUNT))
echo -e "Testes reprovados: ${RED}${FAIL_COUNT}${RESET}"


# --- Limpeza ---
rm -f "$BASH_OUT" "$MINISHELL_OUT" "$BASH_ERR" "$MINISHELL_ERR"
rm -f out.log test_file.txt bash_err_normalized.log minishell_err_normalized.log
echo -e "\n${YELLOW}Limpeza concluída.${RESET}"

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}Todos os testes automatizados passaram com sucesso! Bom trabalho!${RESET}"
else
    echo -e "${RED}Alguns testes falharam. Reveja os logs acima para depurar.${RESET}"
fi
