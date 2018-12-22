
all: main.pb.c config.pb.c

main.pb.c: main.proto
	protoc -I. -Iproto -omain.pb main.proto
	python nanopb_generator.py main.pb

config.pb.c: config.proto
	protoc -I. -Iproto -oconfig.pb config.proto
	python nanopb_generator.py config.pb
