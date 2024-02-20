CC=gcc

CFLAGS=-Wall -Iincludes -Ipath/to/glib/include `pkg-config --cflags glib-2.0`

LDFLAGS=`pkg-config --libs glib-2.0`


all: main testes

main: src/main.o src/parser.o src/passageiros.o src/voos.o src/voos_dados.o src/utilizadores.o src/utilizadores_dados.o src/reservas.o src/reservas_dados.o src/datas.o src/utilidades.o src/queries.o src/interativo.o src/parser_interativo.o src/output.o
	gcc -g src/main.o src/parser.o  src/passageiros.o src/voos.o src/voos_dados.o src/utilizadores.o src/utilizadores_dados.o src/reservas.o src/reservas_dados.o src/datas.o src/utilidades.o src/queries.o src/interativo.o src/parser_interativo.o src/output.o $(LDFLAGS) -o programa-principal -lcurses -lm

testes: testes/testes.o src/parser.o src/passageiros.o src/voos.o src/voos_dados.o src/utilizadores.o src/utilizadores_dados.o src/reservas.o src/reservas_dados.o src/datas.o src/utilidades.o src/queries.o src/output.o
	gcc -g testes/testes.o src/parser.o  src/passageiros.o src/voos.o src/voos_dados.o src/utilizadores.o src/utilizadores_dados.o src/reservas.o src/reservas_dados.o src/datas.o src/utilidades.o src/queries.o src/output.o $(LDFLAGS) -o programa-testes -lcurses -lm

src/main.o: src/main.c

src/parser.o: src/parser.c

src/passageiros.o: src/passageiros.c

src/voos.o: src/voos.c

src/voos_dados.o: src/voos_dados.c

src/utilizadores.o : src/utilizadores.c

src/utilizadores_dados.o: src/utilizadores_dados.c

src/reservas.o: src/reservas.c

src/reservas_dados.o: src/reservas_dados.c

src/datas.o: src/datas.c

src/utilidades.o: src/utilidades.c

src/queries.o: src/queries.c

src/interativo.o: src/interativo.c

src/parser_interativo.o: src/parser_interativo.c

src/output.o: src/output.c

testes/testes.o: testes/testes.c

clean:
	rm -f src/*.o main testes/*.o Resultados/*.txt Resultados/*.csv 