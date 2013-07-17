unix {
  #build the protocol buffer code
  #message(Compiling Protocol Buffer CPP Sources from .proto Files...)
  system(protoc --cpp_out=$$PWD/cpp --proto_path=$$PWD $$PWD/*.proto):HAS_PROTOBUF = TRUE

  isEmpty(HAS_PROTOBUF) {
    error(Failure building proto-buffer sources. Is the google protocol buffer compiler 'protoc' installed?)
  }

  #messages defined through google protocol buffers (as compiled by protoc)
  LIST = $$system(ls $${PWD}/cpp/*.pb.cc)
  for(item, LIST) {
    SOURCES += $${item}
  }

  LIST = $$system(ls $${PWD}/cpp/*.pb.h)
  for(item, LIST) {
    HEADERS += $${item}
  }
}

win32 {
  #build the protocol buffer code
  #message(Compiling Protocol Buffer CPP Sources from .proto Files...)
  LIST = $$system(dir /B *.proto)
  for(item, LIST) {
    system(protoc --cpp_out=$$PWD/cpp --proto_path=$$PWD $$PWD/$${item}):HAS_PROTOBUF = TRUE
  }

  isEmpty(HAS_PROTOBUF) {
    error(Failure building proto-buffer sources. Is the google protocol buffer compiler 'protoc' installed?)
  }

  #messages defined through google protocol buffers (as compiled by protoc)
  LIST = $$system(dir /B cpp\\*.pb.cc)
  for(item, LIST) {
    SOURCES += $${SHARED_DIR}/proto/cpp/$${item}
  }

  LIST = $$system(dir /B cpp\\*.pb.h)
  for(item, LIST) {
    HEADERS += $${SHARED_DIR}/proto/cpp/$${item}
  }
}
