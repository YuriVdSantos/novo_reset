#!/bin/bash

# --- Cores e Utilitários ---
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- Configuração ---
MINISHELL_PATH="./minishell"
BASH_OUT="bash_output.tmp"
MINI_OUT="minishell_output.tmp"
MINI_ERR="minishell_error.tmp"
TEST_FILE="test_file.tmp"
REPORT_FILE="test_report.md"

# --- Funções ---

# Função para inicializar o arquivo de relatório
init_report() {
    rm -f $REPORT_FILE
    echo "# Relatório de Testes do Minishell" > $REPORT_FILE
    echo "Gerado em: $(date)" >> $REPORT_FILE
    echo "" >> $REPORT_FILE
}

# Função para imprimir cabeçalhos de seção
print_header() {
    local title="$1"
    echo -e "\n${BLUE}=======================================================================${NC}" | tee -a $REPORT_FILE
    echo -e "${BLUE}  $title ${NC}" | tee -a $REPORT_FILE
    echo -e "${BLUE}=======================================================================${NC}" | tee -a $REPORT_FILE
    echo -e "\n## $title\n" >> $REPORT_FILE
}

# Função para pausar e aguardar o avaliador, registando no relatório
ask_evaluator() {
    local message="$1"
    echo -e "${YELLOW}AVALIADOR: $message${NC}"
    echo -e "**[AVALIAÇÃO MANUAL]** $message\n" >> $REPORT_FILE
    read -p "Pressione [Enter] para continuar..."
}

# Função para verificar o resultado de um teste e registar no relatório
check_result() {
    COMMAND=$1
    echo -e "Comando: ${YELLOW}${COMMAND}${NC}"
    echo "### Teste: \`$COMMAND\`" >> $REPORT_FILE

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
        echo -e "${GREEN}PASSOU${NC}"
        echo "**Resultado:** <span style='color:green;'>PASSOU</span>" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU${NC}"
        echo "**Resultado:** <span style='color:red;'>FALHOU</span>" >> $REPORT_FILE
        
        [ "$DIFF_OUT" -ne 0 ] && {
            echo -e "  - Saída diverge do Bash."
            echo -e "#### Diferenças na Saída:" >> $REPORT_FILE
            echo -e '```diff' >> $REPORT_FILE
            echo "--- Saída do Bash (stdout)" >> $REPORT_FILE
            cat $BASH_OUT >> $REPORT_FILE
            echo "--- Saída do Minishell (stdout)" >> $REPORT_FILE
            cat $MINI_OUT >> $REPORT_FILE
            echo '```' >> $REPORT_FILE
        }

        [ "$BASH_EXIT_CODE" -ne "$MINI_EXIT_CODE" ] && {
            echo -e "  - Exit Code diverge (Bash: $BASH_EXIT_CODE, Minishell: $MINI_EXIT_CODE)"
            echo -e "#### Diferença no Exit Code:\n* **Bash:** \`$BASH_EXIT_CODE\`\n* **Minishell:** \`$MINI_EXIT_CODE\`" >> $REPORT_FILE
        }

        [ -s $MINI_ERR ] && {
            local errmsg
            errmsg=$(cat $MINI_ERR)
            echo -e "  - Minishell gerou erro:\n$errmsg"
            echo -e "#### Saída de Erro (stderr) do Minishell:\n\`\`\`\n$errmsg\n\`\`\`" >> $REPORT_FILE
        }
        ask_evaluator "Analise a falha documentada no relatório e na consola."
    fi
    echo "---" >> $REPORT_FILE
}


# --- Seções de Teste ---

test_compilation() {
    print_header "1. Testes de Compilação"
    make re > /dev/null
    
    if ! command -v make &> /dev/null; then
        echo -e "${RED}Erro: 'make' não encontrado. Por favor, instale-o.${NC}" | tee -a $REPORT_FILE
        exit 1
    fi
    
    if [ ! -f "Makefile" ]; then
        echo -e "${RED}Erro: Makefile não encontrado no diretório atual.${NC}" | tee -a $REPORT_FILE
        exit 1
    fi
    
    echo "Verificando flags de compilação (-Wall -Wextra -Werror)..." | tee -a $REPORT_FILE
    if make -n | grep -E -- "-Wall -Wextra -Werror"; then
        echo -e "${GREEN}PASSOU: Flags encontradas.${NC}" | tee -a $REPORT_FILE
        echo "- Flags de compilação: <span style='color:green;'>OK</span>" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: Flags -Wall -Wextra -Werror não encontradas na saída de 'make -n'.${NC}" | tee -a $REPORT_FILE
        echo "- Flags de compilação: <span style='color:red;'>FALHOU</span>" >> $REPORT_FILE
    fi

    echo "Compilando o minishell..." | tee -a $REPORT_FILE
    make > make_log.tmp 2>&1
    if [ -x "$MINISHELL_PATH" ]; then
        echo -e "${GREEN}PASSOU: $MINISHELL_PATH foi compilado com sucesso.${NC}" | tee -a $REPORT_FILE
        echo "- Compilação: <span style='color:green;'>OK</span>" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: $MINISHELL_PATH não foi encontrado ou não é executável após 'make'.${NC}" | tee -a $REPORT_FILE
        echo "- Compilação: <span style='color:red;'>FALHOU</span>" >> $REPORT_FILE
        echo -e "#### Log de Compilação:\n\`\`\`\n$(cat make_log.tmp)\n\`\`\`" >> $REPORT_FILE
        exit 1
    fi

    echo "Verificando se o Makefile não revincula..." | tee -a $REPORT_FILE
    if make | grep -q "Nothing to be done"; then
        echo -e "${GREEN}PASSOU: Makefile não revinculou.${NC}" | tee -a $REPORT_FILE
        echo "- Re-vinculação: <span style='color:green;'>OK</span>" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: Makefile foi revinculado desnecessariamente.${NC}" | tee -a $REPORT_FILE
        echo "- Re-vinculação: <span style='color:red;'>FALHOU</span>" >> $REPORT_FILE
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
}

test_builtins() {
    print_header "4. Testes de Built-ins (echo, pwd, cd, env)"
    check_result "echo hello world"
    check_result "echo -n hello"
    check_result "pwd"
    check_result "env"
    
    echo "Testando 'cd'..." | tee -a $REPORT_FILE
    mkdir -p test_dir
    (cd test_dir && echo "cd . && pwd" | ../$MINISHELL_PATH) > $MINI_OUT
    if grep -q "test_dir" $MINI_OUT; then
         echo -e "${GREEN}PASSOU${NC} (cd e pwd parecem funcionar juntos)" | tee -a $REPORT_FILE
         echo -e "### Teste: \`cd\`\n**Resultado:** <span style='color:green;'>PASSOU</span>\n---" >> $REPORT_FILE
    else
         echo -e "${RED}FALHOU${NC} (cd e pwd não funcionaram como esperado)" | tee -a $REPORT_FILE
         echo -e "### Teste: \`cd\`\n**Resultado:** <span style='color:red;'>FALHOU</span>\n---" >> $REPORT_FILE
    fi
    rm -rf test_dir
}

test_exit() {
    print_header "5. Teste do 'exit'"
    echo -e "\nTestando 'exit 42'..." | tee -a $REPORT_FILE
    echo "exit 42" | $MINISHELL_PATH
    if [ $? -eq 42 ]; then
        echo -e "${GREEN}PASSOU: 'exit 42' retornou 42.${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: \`exit 42\`\n**Resultado:** <span style='color:green;'>PASSOU</span>\n---" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: 'exit 42' retornou $?. Esperado: 42.${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: \`exit 42\`\n**Resultado:** <span style='color:red;'>FALHOU</span>\n---" >> $REPORT_FILE
    fi

    echo -e "\nTestando 'exit' (sem argumentos)..." | tee -a $REPORT_FILE
    echo "ls non_existent_file; exit" | $MINISHELL_PATH > /dev/null 2>&1
    MINI_EXIT_CODE=$?
    bash -c "ls non_existent_file; exit" > /dev/null 2>&1
    BASH_EXIT_CODE=$?
    if [ $MINI_EXIT_CODE -eq $BASH_EXIT_CODE ]; then
        echo -e "${GREEN}PASSOU: 'exit' sem argumentos retornou o status do último comando ($MINI_EXIT_CODE).${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: \`exit\` sem args\n**Resultado:** <span style='color:green;'>PASSOU</span>\n---" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: 'exit' sem argumentos retornou $MINI_EXIT_CODE. Esperado: $BASH_EXIT_CODE.${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: \`exit\` sem args\n**Resultado:** <span style='color:red;'>FALHOU</span>\n---" >> $REPORT_FILE
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
    
    echo "Testando caminho relativo..." | tee -a $REPORT_FILE
    check_result "./minishell"
    
    echo "Testando \$PATH..." | tee -a $REPORT_FILE
    check_result "ls"
    echo "Testando com PATH indefinido..." | tee -a $REPORT_FILE
    (export PATH="" && echo "ls" | $MINISHELL_PATH > $MINI_OUT 2> $MINI_ERR)
    if [ -s $MINI_ERR ]; then
        echo -e "${GREEN}PASSOU: Comando falhou como esperado com PATH vazio.${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: PATH vazio\n**Resultado:** <span style='color:green;'>PASSOU</span>\n---" >> $REPORT_FILE
    else
        echo -e "${RED}FALHOU: Comando funcionou com PATH vazio, o que não deveria ocorrer.${NC}" | tee -a $REPORT_FILE
        echo -e "### Teste: PATH vazio\n**Resultado:** <span style='color:red;'>FALHOU</span>\n---" >> $REPORT_FILE
    fi
}

test_redirections() {
    print_header "10. Testes de Redirecionamento"
    echo "hello from test file" > $TEST_FILE
    
    check_result "cat < $TEST_FILE"
    
    echo "Testando > e >>..." | tee -a $REPORT_FILE
    echo "echo line1 > outfile.tmp && cat outfile.tmp" | $MINISHELL_PATH > $MINI_OUT
    echo "echo line2 >> outfile.tmp && cat outfile.tmp" | $MINISHELL_PATH >> $MINI_OUT
    if grep "line1" $MINI_OUT && grep "line2" $MINI_OUT; then
        echo -e "${GREEN}PASSOU: Redirecionamentos > e >> funcionaram.${NC}" | tee -a $REPORT_FILE
    else
        echo -e "${RED}FALHOU: Redirecionamentos > e >> falharam.${NC}" | tee -a $REPORT_FILE
    fi

    echo "Testando Here Document (<<)..." | tee -a $REPORT_FILE
    check_result "cat << EOF
heredoc line 1
heredoc line 2
EOF"
}

test_pipes() {
    print_header "11. Testes de Pipes"
    check_result "ls -l | grep .c | wc -l"
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
    
    echo "Testando sinal em processo filho (simulando ctrl-C)..." | tee -a $REPORT_FILE
    (sleep 5 | $MINISHELL_PATH) &
    MINI_PID=$!
    sleep 1
    kill -INT $MINI_PID
    wait $MINI_PID 2>/dev/null
    local exit_code=$?
    if [ $exit_code -eq 130 ]; then
        echo -e "${GREEN}PASSOU: Processo filho terminou com exit code 130 após SIGINT.${NC}" | tee -a $REPORT_FILE
    else
        echo -e "${RED}FALHOU: Processo filho terminou com exit code $exit_code. Esperado 130.${NC}" | tee -a $REPORT_FILE
    fi

    check_result "akjshdgakjsdghaksjd"
    check_result "cat | cat | ls"
    ask_evaluator "TESTE MANUAL: Navegue pelo histórico com as setas para cima e para baixo. Verifique se funciona."
}


# --- Execução Principal ---
cleanup() {
    rm -f $BASH_OUT $MINI_OUT $MINI_ERR $TEST_FILE outfile.tmp make_log.tmp
}

trap cleanup EXIT

init_report
test_compilation
test_simple_commands
test_arguments
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

echo -e "\n${GREEN}Fim dos testes! Relatório gerado em: $REPORT_FILE${NC}"
