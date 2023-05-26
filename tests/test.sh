#!/bin/bash
echo "Sending " $1
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl  --data-binary   @$1  http://localhost:1337&\
curl  --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:4242&\
curl -H "Transfer-Encoding: chunked" --data-binary   @$1  http://localhost:1337 && >&2 echo "Done"

