docker run --rm  -v $(pwd):/tmp/eosfactory/contracts/ -t maxpetriev/eos_testing_env:1.0.1 /bin/bash -c ' python3 unittests/main.py'

