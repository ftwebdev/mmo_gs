###################################
########## main programm ##########
###################################

mmogs: ./lib/linux/libftwd-global.so ./lib/linux/libftwd-progresbar.so ./obj/mmogs.o
	g++ ./obj/mmogs.o -I./includes/ -L./lib/linux/ -lftwd-global -lftwd-exc -lftwd-progresbar -lftwd-excmsgs -o mmogs

./obj/mmogs.o:
	g++ -c ./source/mmogs.cpp -I./includes/ -std=c++11 -o ./obj/mmogs.o

#################################
########## shared libs ##########
#################################

# libftwd-excmsgs.so
#locale-errmsgs-utf8-rus for Russian, locale-errmsgs-utf8-eng for English error messages
./obj/locale-errmsgs.o: locale-errmsgs-utf8-rus

./lib/linux/libftwd-excmsgs.so: ./obj/locale-errmsgs.o
	g++ -shared ./obj/locale-errmsgs.o -I./includes/ -o ./lib/linux/libftwd-excmsgs.so


locale-errmsgs-utf8-eng: ./obj/ErrMsgs-utf8ru.o
	g++ -fPIC -std=c++11 -c ./lib/exception/ErrMsgs-utf8-eng.cpp -I./includes/ -o ./obj/locale-errmsgs.o
locale-errmsgs-utf8-rus:
	g++ -fPIC -std=c++11 -c ./lib/exception/ErrMsgs-utf8-rus.cpp -I./includes/ -o ./obj/locale-errmsgs.o


# libftwd-exc.so
./lib/linux/libftwd-exc.so: ./lib/linux/libftwd-excmsgs.so ./obj/exception.o
	g++ -shared ./obj/exception.o -L./lib/linux/ -lftwd-excmsgs -o ./lib/linux/libftwd-exc.so

./obj/exception.o: ./lib/linux/libftwd-excmsgs.so
	g++ -fPIC -std=c++11 -c ./lib/exception/exception.cpp -I./includes/ -o ./obj/exception.o

# libftwd-global.so
./lib/linux/libftwd-global.so: ./lib/linux/libftwd-exc.so ./obj/global.o
	g++ -shared ./obj/global.o -I./includes/ -L./lib/linux/ -lftwd-exc -lftwd-excmsgs -o ./lib/linux/libftwd-global.so

./obj/global.o:
	g++ -fPIC -std=c++11 -c ./lib/global/global.cpp -I./includes/ -o ./obj/global.o

./lib/linux/libftwd-progresbar.so: ./obj/progresbar.o
	g++ -shared ./obj/progresbar.o -I./includes/ -L./lib/linux/ -lftwd-exc -lftwd-global -o ./lib/linux/libftwd-progresbar.so

./obj/progresbar.o:
	g++ -fPIC -std=c++11 -c ./lib/progresbar/progresbar.cpp -I./includes/ -o ./obj/progresbar.o

#####################################################################
########### installing(copying shared libs to '/usr/lib') ###########
#####################################################################
install:
	sudo cp ./lib/linux/libftwd-*.so /usr/lib/

#####################################################################
######## uninstalling(removing shared libs from '/usr/lib') #########
#####################################################################
uninstall:
	sudo rm -f /usr/lib/libftwd-*.so

####################################################
########### full clean with uninstalling ###########
####################################################
clean: uninstall
	rm -rf *.so *.o ./obj/*.o ./lib/linux/*.so mmogs

