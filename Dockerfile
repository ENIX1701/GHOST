# === BUILD STAGE ===
FROM alpine:edge AS builder

RUN apk add --no-cache g++ git cmake make linux-headers openssl-dev meson

WORKDIR /src

COPY . . 

RUN cmake . -DSHADOW_URL=${SHADOW_URL:-127.0.0.1} -DSHADOW_PORT=${SHADOW_PORT:-9999} -DENABLE_DEBUG=ON -DBUILD_SHARED_LIBS=OFF && make -j$(nproc)

# === RUNTIME ===
FROM alpine:edge

RUN apk add --no-cache libstdc++ openssl ca-certificates

WORKDIR /app

COPY --from=builder /src/bin/Ghost .

CMD ["./Ghost"]
