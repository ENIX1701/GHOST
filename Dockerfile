FROM alpine:edge

RUN apk add --no-cache g++ cmake make linux-headers

WORKDIR /src

COPY . . 

CMD ["/bin/sh", "-c", "cmake . -DSHADOW_URL=${SHADOW_URL:-shadow} -DSHADOW_PORT=${SHADOW_PORT:-9999} -DENABLE_DEBUG=ON && amke -j4 && ./bin/Ghost"]