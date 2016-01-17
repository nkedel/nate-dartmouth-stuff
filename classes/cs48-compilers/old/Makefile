CC = g++
CFLAGS = -g
BISONFL = -d -v

.SUFFIXES: .cc

.cc.o:
	$(CC) $(CFLAGS) -c $<

tree : lexyy.o tree.tab.o TreeNode.o TreeMain.o
        $(CC) -o $@ $(CFLAGS) lexyy.o tree.tab.o TreeNode.o TreeMain.o -lfl

lexyy.o : lexyy.c
	gcc -c $(CFLAGS) $<

tree.tab.o : tree.tab.c
	$(CC) -c $(CFLAGS) $<

lexyy.c : tree.l tree.tab.h
	flex tree.l

tree.tab.h : tree.y
	bison $(BISONFL) $<

tree.tab.c : tree.y
	bison $(BISONFL) $<

TreeNode.o : TreeNode.h TreeNode.cc

TreeMain.o : TreeNode.h TreeMain.cc

clean :
        rm -f tree lexyy.o tree.tab.o TreeNode.o TreeMain.o \
        tree.tab.h tree.tab.c lexyy.c

depend :
	makedepend -- $(CFLAGS) -- TreeNode.cc TreeMain.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.

TreeNode.o: /usr/include/stdlib.h /usr/include/standards.h
TreeNode.o: /usr/include/stdio.h /usr/include/string.h
TreeNode.o: /usr/include/sys/types.h TreeNode.h
TreeMain.o: /usr/include/stdlib.h /usr/include/standards.h
TreeMain.o: /usr/include/stdio.h TreeNode.h
