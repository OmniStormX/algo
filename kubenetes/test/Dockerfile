# ---- Build Stage ----
FROM golang:1.23-alpine AS builder

WORKDIR /app

# Copy source code
COPY main.go .

# Init module and build a static binary (no external deps needed)
RUN go mod init kubenetes \
    && CGO_ENABLED=0 GOOS=linux GOARCH=amd64 \
       go build -ldflags="-s -w" -o /app/server .

# ---- Runtime Stage ----
FROM alpine:3.21

RUN apk add --no-cache ca-certificates

WORKDIR /app
COPY --from=builder /app/server .

EXPOSE 6763

ENV POD_VERSION=[V0]

CMD ["./server"]
