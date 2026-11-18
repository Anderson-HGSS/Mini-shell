# Mini-shell

Um interpretador de comandos (*Shell*) desenvolvido em linguagem C, com o objetivo de reproduzir funcionalidades básicas de um terminal Unix/Linux.

O projeto implementa a leitura, interpretação e execução de comandos, permitindo a interação com o sistema operacional por meio de uma interface de linha de comando própria.

Além da execução de programas externos, o Mini-shell oferece suporte a comandos internos, redirecionamento de entrada e saída, execução em segundo plano e pipelines, utilizando recursos do sistema operacional para gerenciar processos.

## 🚀 Começando

Estas instruções permitem obter uma cópia do projeto e executá-lo em sua máquina local para fins de utilização, desenvolvimento e testes.

### 📋 Pré-requisitos

Para compilar e executar o projeto, são necessários:

- **Sistema operacional:** Linux ou outro ambiente compatível com POSIX.
- **Compilador:** GCC.
- **Ferramenta de compilação:** Make.
- **Padrão da linguagem:** C11.

### 🔧 Instalação

**1. Clone o repositório:**

```bash
git clone https://github.com/Anderson-HGSS/Mini-shell.git
```

**2. Acesse o diretório do projeto:**

```bash
cd Mini-shell
```

**3. Compile o projeto utilizando o Makefile:**

```bash
make
```

Após a compilação, será gerado o executável `mysh`.

**4. Execute o Mini-shell:**

```bash
./mysh
```

O terminal apresentará o seguinte prompt:

```bash
mysh>
```

A partir desse momento, você poderá inserir comandos e interagir com o sistema.

Para encerrar o Mini-shell, utilize o comando:

```bash
exit
```

Também é possível finalizar a entrada utilizando `Ctrl + D`.

## ⚙️ Funcionalidades

O Mini-shell implementa funcionalidades fundamentais para o funcionamento de um interpretador de comandos:

- **Execução de comandos externos:** permite executar programas disponíveis no sistema utilizando `execvp()`.
- **Comandos internos (Built-ins):** implementação dos comandos `cd`, `pwd` e `exit`.
- **Pipelines (`|`):** permite conectar a saída de um comando à entrada de outro.
- **Redirecionamento de entrada (`<`):** permite utilizar arquivos como entrada de comandos.
- **Redirecionamento de saída (`>` e `>>`):** permite direcionar a saída dos comandos para arquivos.
- **Execução em segundo plano (`&`):** permite executar processos sem bloquear o terminal.
- **Suporte a aspas duplas:** possibilita interpretar argumentos que contêm espaços como um único token.
- **Reexecução do último comando (`!!`):** permite executar novamente o comando anterior.
- **Tratamento de sinais:** gerenciamento de sinais como `SIGINT` e `SIGCHLD`, incluindo o comportamento de `Ctrl + C`.
- **Tratamento de erros:** identificação de erros de sintaxe e falhas durante a execução dos comandos.

## 💻 Exemplos de utilização

Abaixo estão alguns exemplos de comandos que podem ser executados no Mini-shell.

**1. Executar um comando externo:**

```bash
mysh> ls
```

**2. Consultar o diretório atual:**

```bash
mysh> pwd
```

**3. Navegar entre diretórios:**

```bash
mysh> cd /home
```

**4. Utilizar pipelines:**

```bash
mysh> ls | grep txt
```

**5. Redirecionar a saída para um arquivo:**

```bash
mysh> ls > arquivos.txt
```

**6. Executar um processo em segundo plano:**

```bash
mysh> sleep 10 &
```

**7. Reexecutar o último comando:**

```bash
mysh> !!
```

## 🛠️ Construído com

As principais tecnologias e ferramentas utilizadas no desenvolvimento foram:

- **C** — Linguagem utilizada na implementação do interpretador.
- **GCC** — Compilador responsável pela geração do executável.
- **Make** — Automatização do processo de compilação por meio de um Makefile.
- **POSIX** — Interface utilizada para gerenciamento de processos, sinais, arquivos e comunicação entre processos.

## 📁 Estrutura do projeto

O projeto está organizado da seguinte maneira:

```text
Mini-shell/
│
├── src/
│   ├── main.c
│   ├── parser.c
│   ├── executor.c
│   └── builtins.c
│
├── testes/
│
├── Makefile
└── README.md
```

**Descrição dos principais módulos:**

- `main.c`: contém a função principal, responsável pelo loop de leitura de comandos, integração dos módulos e tratamento de sinais.
- `parser.c`: responsável pela tokenização e interpretação dos comandos informados pelo usuário.
- `executor.c`: realiza a execução de programas externos, criação de processos, pipelines e redirecionamentos.
- `builtins.c`: implementa os comandos internos do interpretador, como `cd`, `pwd` e `exit`.
- `testes/`: diretório destinado aos testes do projeto.
- `Makefile`: automatiza a compilação e a limpeza dos arquivos gerados.

## 🧪 Executando os testes

O projeto possui um diretório destinado aos testes. Para recompilar o programa e realizar verificações manuais, utilize:

```bash
make clean
make
./mysh
```

Durante os testes, recomenda-se verificar:

- Execução de comandos externos.
- Funcionamento dos comandos internos.
- Interpretação de argumentos entre aspas.
- Execução de pipelines.
- Redirecionamento de entrada e saída.
- Execução de processos em segundo plano.
- Comportamento diante de erros de sintaxe.
- Tratamento de sinais e encerramento do programa.

## 🧹 Limpeza dos arquivos

Para remover os arquivos objeto e o executável gerados durante a compilação, execute:

```bash
make clean
```

## ✒️ Autores

Projeto desenvolvido em parceria por:

- **Anderson Henrique** — [GitHub](https://github.com/Anderson-HGSS)
- **Vitor Hugo Vaz** — [GitHub](https://github.com/ra143349)

O desenvolvimento foi realizado de forma colaborativa, envolvendo a implementação das funcionalidades e a integração dos diferentes módulos do interpretador.

---

Desenvolvido com ❤️ e C.
