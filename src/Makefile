RES_DIR = resourses
DOC_DIR = doc
BUILD_DIR = build
INSTALL_DIR = install
CC := g++
CFLAGS := -std=c++17 -Wall -Werror -Wextra



all: tests

res:
	mkdir -p $(RES_DIR)

install: install_cli install_desk

install_desk: tetris.a snake.a
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ../ && make
	mkdir -p $(INSTALL_DIR)
	mv $(BUILD_DIR)/Desk $(INSTALL_DIR)


uninstall: clean
	rm -rf $(INSTALL_DIR)

dist:
	zip -r ../../CPP3_BrickGame_v2.0-1.zip ../../CPP3_BrickGame_v2.0-1 

tests: clean tetris.a snake.a
	$(CC) $(CFLAGS) test/*.cc -L. tetris.a -L. snake.a -o Test -lgtest
	./Test

gcov_report: clean res
	g++ $(CFLAGS) --coverage -c brick_game/tetris/tetrisModel.cc
	mv tetrisModel.o $(RES_DIR)/
	ar rcs tetris.a $(RES_DIR)/tetrisModel.o
	ranlib tetris.a
	g++ $(CFLAGS) --coverage -c brick_game/snake/snake.cc
	mv snake.o $(RES_DIR)/
	ar rcs snake.a $(RES_DIR)/snake.o
	ranlib snake.a
	g++ $(CFLAGS) -c test/*.cc
	mv *.o $(RES_DIR)/
	g++ $(CFLAGS) --coverage $(RES_DIR)/test.o -L. tetris.a -L. snake.a -o gcov_test -lgtest
	mkdir -p coverage_report
	./gcov_test
	lcov -c --directory . --output-file tetris.info
	genhtml tetris.info --output-directory coverage_report
	mv *.gcda *.gcno *.info $(RES_DIR)/

tetris.a: res
	$(CC) $(CFLAGS) -c brick_game/tetris/tetrisModel.cc
	mv tetrisModel.o $(RES_DIR)/
	ar rcs tetris.a $(RES_DIR)/tetrisModel.o
	ranlib tetris.a

snake.a: res
	$(CC) $(CFLAGS) -c brick_game/snake/snake.cc
	mv snake.o $(RES_DIR)/
	ar rcs snake.a $(RES_DIR)/snake.o
	ranlib snake.a

#Naschet utechek:
#https://invisible-island.net/ncurses/ncurses.faq.html#config_leaks

clean:
	rm -rf $(DOC_DIR) $(RES_DIR) $(BUILD_DIR) coverage_report *.o *.a gcov_test tetris_high_score.txt snake_high_score.txt Test

dvi:
	mkdir -p $(DOC_DIR)
	doxygen Doxyfile

install_cli: tetris.a snake.a
	mkdir -p $(INSTALL_DIR)
	$(CC) $(CFLAGS) -c start_game/start_cli.cc gui/cli/view.cc -lstdc++ -lncurses
	$(CC) $(CFLAGS) *.o -L. tetris.a -L. snake.a -o Cli -lstdc++ -lncurses
	mv *.o $(RES_DIR)/
#	g++ -std=c++17 start_game/start_cli.cc gui/cli/view.cc brick_game/snake/snake.cc brick_game/tetris/tetrisModel.cc -o Cli -lstdc++ -lncurses
	mv Cli $(INSTALL_DIR)/
	echo '0' > $(INSTALL_DIR)/tetris_high_score.txt
	echo '0' > $(INSTALL_DIR)/snake_high_score.txt

vg_cli: install_cli
	valgrind  --tool=memcheck --track-fds=yes --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./install/Cli