.PHONY: all
all: bin/server

bin/server: src/main.c src/server.c
	mkdir -p bin/
	gcc src/main.c src/server.c -o bin/server

clean:
	rm -r bin/*

.PHONY: create-service
create-systemd-service: src/main.c src/server.c
	mkdir -p bin/
	gcc src/main.c src/server.c -o bin/server
	cp -r bin/server /usr/local/bin
remove-systemd-service:
	rm /usr/local/bin/server

.PHONY: example-website
example-website:
	cp -r example/* /var/www/html
remove-example-website:
	rm /var/www/html/index.html
	rm /var/www/html/styles.css

-include $(DEPS)
