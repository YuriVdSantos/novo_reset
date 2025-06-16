#!/bin/bash

# --- Cores e Utilitários ---
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

MINISHELL_PATH="./minishell"
BASH_OUT="bash_output.tmp"
MINI_OUT="minishell_output.tmp"
MINI_ERR="minishell_error.tmp"
TEST_FILE="test_file.tmp"

# Função para imprimir cabeçalhos de seção
print_header() {
    echo -e "\n${BLUE}=======================================================================${NC}"
    echo -e "${BLUE}  $1 ${NC}"
    echo -e "${BLUE}=======================================================================${NC}"
}

# Função para pausar e aguardar o avaliador
ask_evaluator() {
    echo -e "${YELLOW}AVALIADOR: $1${NC}"
    read -p "Pressione [Enter] para continuar..."
}

# Função para verificar o resultado de um teste
check_result() {
    COMMAND=$1
    echo -e "Comando: ${YELLOW}${COMMAND}${NC}"
    # Executa no Bash para obter a referência
    eval "bash -c '${COMMAND}'" > $BASH_OUT 2>&1
    BASH_EXIT_CODE=$?

    # Executa no Minishell
    echo "${COMMAND}" | $MINISHELL_PATH > $MINI_OUT 2> $MINI_ERR
    MINI_EXIT_CODE=$?

    # Compara saídas e códigos de exit
    diff $BASH_OUT $MINI_OUT > /dev/null
    DIFF_OUT=$?

    if [ "$DIFF_OUT" -eq 0 ] && [ "$BASH_EXIT_CODE" -eq "$MINI_EXIT_CODE" ]; then
        echo -e "${GREEN}PASSOU${NC} (Saída e Exit Code correspondem ao Bash)"
    else
        echo -e "${RED}FALHOU${NC}"
        [ "$DIFF_OUT" -ne 0 ] && echo -e "  - Saída diverge do Bash." && ask_evaluator "Verifique as diferenças:\nBASH:\n$(cat $BASH_OUT)\nMINISHELL:\n$(cat $MINI_OUT)"
        [ "$BASH_EXIT_CODE" -ne "$MINI_EXIT_CODE" ] && echo -e "  - Exit Code diverge (Bash: $BASH_EXIT_CODE, Minishell: $MINI_EXIT_CODE)"
        [ -s $MINI_ERR ] && echo -e "  - Minishell gerou erro:\n$(cat $MINI_ERR)"
        ask_evaluator "Analise a falha acima."
    fi
    echo "----------------------------------------"
}


# --- Seções de Teste ---

test_compilation() {
    print_header "1. Testes de Compilação"
    make re > /dev/null
    
    if ! command -v make &> /dev/null; then
        echo -e "${RED}Erro: 'make' não encontrado. Por favor, instale-o.${NC}"
        exit 1
    fi
    
    if [ ! -f "Makefile" ]; then
        echo -e "${RED}Erro: Makefile não encontrado no diretório atual.${NC}"
        exit 1
    fi
    
    echo "Verificando flags de compilação (-Wall -Wextra -Werror)..."
    if make -n | grep -E -- "-Wall -Wextra -Werror"; then
        echo -e "${GREEN}PASSOU: Flags encontradas.${NC}"
    else
        echo -e "${RED}FALHOU: Flags -Wall -Wextra -Werror não encontradas na saída de 'make -n'.${NC}"
    fi

    echo "Compilando o minishell..."
    make > /dev/null
    if [ -x "$MINISHELL_PATH" ]; then
        echo -e "${GREEN}PASSOU: $MINISHELL_PATH foi compilado com sucesso.${NC}"
    else
        echo -e "${RED}FALHOU: $MINISHELL_PATH não foi encontrado ou não é executável após 'make'.${NC}"
        exit 1
    fi

    echo "Verificando se o Makefile não revincula..."
    if make | grep -q "Nothing to be done"; then
        echo -e "${GREEN}PASSOU: Makefile não revinculou.${NC}"
    else
        echo -e "${RED}FALHOU: Makefile foi revinculado desnecessariamente.${NC}"
    fi
}

test_simple_commands() {
    print_header "2. Comando Simples e Variáveis Globais"
    check_result "/bin/ls"
    check_result " "
    check_result ""
    ask_evaluator "Pergunte ao aluno sobre o uso de variáveis globais e peça para justificar."
}

test_arguments() {
    print_header "3. Testes de Argumentos"
    check_result "/bin/ls -l -a"
    check_result "wc -l -w -c"
}

test_builtins() {
    print_header "4. Testes de Built-ins (echo, pwd, cd, env)"
    check_result "echo hello world"
    check_result "echo -n hello"
    check_result "echo -n -n -n hello"
    check_result "pwd"
    check_result "env"
    
    echo "Testando 'cd'..."
    mkdir -p test_dir
    (cd test_dir && echo "cd ../test_dir && pwd" | ../$MINISHELL_PATH) > $MINI_OUT
    if grep -q "test_dir" $MINI_OUT; then
         echo -e "${GREEN}PASSOU${NC} (cd e pwd parecem funcionar juntos)"
    else
         echo -e "${RED}FALHOU${NC} (cd e pwd não funcionaram como esperado)"
    fi
    rm -rf test_dir
}

test_exit() {
    print_header "5. Teste do 'exit'"
    echo "Testando 'exit 42'..."
    echo "exit 42" | $MINISHELL_PATH
    if [ $? -eq 42 ]; then
        echo -e "${GREEN}PASSOU: 'exit 42' retornou 42.${NC}"
    else
        echo -e "${RED}FALHOU: 'exit 42' retornou $?. Esperado: 42.${NC}"
    fi

    echo "Testando 'exit' (sem argumentos)..."
    echo "ls non_existent_file; exit" | $MINISHELL_PATH > /dev/null 2>&1
    MINI_EXIT_CODE=$?
    bash -c "ls non_existent_file; exit" > /dev/null 2>&1
    BASH_EXIT_CODE=$?
    if [ $MINI_EXIT_CODE -eq $BASH_EXIT_CODE ]; then
        echo -e "${GREEN}PASSOU: 'exit' sem argumentos retornou o status do último comando ($MINI_EXIT_CODE).${NC}"
    else
        echo -e "${RED}FALHOU: 'exit' sem argumentos retornou $MINI_EXIT_CODE. Esperado: $BASH_EXIT_CODE.${NC}"
    fi
}

test_return_value() {
    print_header "6. Teste de Valor de Retorno (\$?)"
    check_result "ls; echo \$?"
    check_result "ls non_existent_file; echo \$?"
}

test_quotes() {
    print_header "7. Testes de Aspas (Simples e Duplas)"
    check_result "echo \"hello   world\""
    check_result "echo 'hello   world'"
    check_result "echo \"'cat lol.c' | 'cat > lol.c'\""
    check_result "echo 'echo \$USER'"
    check_result "echo \"echo \$USER\""
}

test_export_unset() {
    print_header "8. Testes de 'export' e 'unset'"
    check_result "export TEST_VAR=minishell && env | grep TEST_VAR"
    check_result "export TEST_VAR=minishell && export TEST_VAR=rocks && env | grep TEST_VAR"
    check_result "export TEST_VAR=minishell && unset TEST_VAR && env | grep TEST_VAR"
}

test_paths() {
    print_header "9. Testes de Caminhos (Relativo e de Ambiente)"
    
    echo "Testando caminho relativo..."
    mkdir -p temp/test
    cp $MINISHELL_PATH temp/test/
    (cd temp/test && echo "./minishell" | ./minishell) > /dev/null
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}PASSOU: Execução com caminho relativo funcionou.${NC}"
    else
        echo -e "${RED}FALHOU: Execução com caminho relativo falhou.${NC}"
    fi
    rm -rf temp

    echo "Testando $PATH..."
    check_result "ls"
    echo "Testando com PATH indefinido..."
    (export PATH="" && echo "ls" | $MINISHELL_PATH > $MINI_OUT 2> $MINI_ERR)
    if [ -s $MINI_ERR ]; then
        echo -e "${GREEN}PASSOU: Comando falhou como esperado com PATH vazio.${NC}"
    else
        echo -e "${RED}FALHOU: Comando funcionou com PATH vazio, o que não deveria ocorrer.${NC}"
    fi
}

test_redirections() {
    print_header "10. Testes de Redirecionamento"
    echo "hello from test file" > $TEST_FILE
    
    check_result "cat < $TEST_FILE"
    check_result "grep hello < $TEST_FILE"
    
    echo "Testando > e >>..."
    echo "echo line1 > outfile.tmp && cat outfile.tmp" | $MINISHELL_PATH > $MINI_OUT
    echo "echo line2 >> outfile.tmp && cat outfile.tmp" | $MINISHELL_PATH >> $MINI_OUT
    if grep "line1" $MINI_OUT && grep "line2" $MINI_OUT; then
        echo -e "${GREEN}PASSOU: Redirecionamentos > e >> funcionaram.${NC}"
    else
        echo -e "${RED}FALHOU: Redirecionamentos > e >> falharam.${NC}"
        cat $MINI_OUT
    fi

    echo "Testando Here Document (<<)..."
    echo -e "cat << EOF\nheredoc line 1\nheredoc line 2\nEOF" | $MINISHELL_PATH > $MINI_OUT
    if grep "heredoc line 1" $MINI_OUT && grep "heredoc line 2" $MINI_OUT; then
        echo -e "${GREEN}PASSOU: Here document (<<) funcionou.${NC}"
    else
        echo -e "${RED}FALHOU: Here document (<<) falhou.${NC}"
        cat $MINI_OUT
    fi
}

test_pipes() {
    print_header "11. Testes de Pipes"
    check_result "ls -l | grep .c | wc -l"
    check_result "cat $TEST_FILE | grep hello"
    check_result "ls non_existent_file | grep bla"
}

test_env_vars() {
    print_header "12. Testes de Variáveis de Ambiente"
    check_result "echo \$PATH"
    check_result "echo \"Your user is: \$USER\""
}


test_signals_and_crazy() {
    print_header "13. Sinais e Testes Finais"
    ask_evaluator "TESTE MANUAL: Pressione ctrl-C em um prompt vazio. Deve mostrar um novo prompt."
    ask_evaluator "TESTE MANUAL: Pressione ctrl-\\ em um prompt vazio. Não deve fazer nada."
    ask_evaluator "TESTE MANUAL: Pressione ctrl-D em um prompt vazio. O minishell deve fechar. RELANCE O SCRIPT APÓS ESTE TESTE."
    ask_evaluator "TESTE MANUAL: Digite 'ls -l' e pressione ctrl-C antes de dar Enter. O buffer deve ser limpo."
    
    echo "Testando sinal em processo filho (simulando ctrl-C)..."
    # Inicia o minishell com um comando que bloqueia
    (sleep 5 | $MINISHELL_PATH) &
    MINI_PID=$!
    sleep 1 # Dá tempo para o minishell iniciar
    # Envia o sinal de interrupção (como ctrl-C)
    kill -INT $MINI_PID
    wait $MINI_PID 2>/dev/null
    if [ $? -eq 130 ]; then
        echo -e "${GREEN}PASSOU: Processo filho terminou com exit code 130 após SIGINT.${NC}"
    else
        echo -e "${RED}FALHOU: Processo filho não terminou com o exit code correto após SIGINT.${NC}"
    fi

    check_result "akjshdgakjsdghaksjd"
    check_result "cat | cat | ls"
    ask_evaluator "TESTE MANUAL: Navegue pelo histórico com as setas para cima e para baixo. Verifique se funciona."
}


# --- Execução Principal ---
cleanup() {
    rm -f $BASH_OUT $MINI_OUT $MINI_ERR $TEST_FILE outfile.tmp
}

trap cleanup EXIT

test_compilation
test_simple_commands
test_builtins
test_exit
test_return_value
test_quotes
test_export_unset
test_paths
test_redirections
test_pipes
test_env_vars
test_signals_and_crazy

echo -e "\n${GREEN}Fim dos testes!${NC}"
