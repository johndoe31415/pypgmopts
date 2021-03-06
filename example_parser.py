import argparse
parser = argparse.ArgumentParser(prog = "knitserver", description = "Brother KH-930 knitting server", add_help = False)
parser.add_argument("-m", "--mode", choices = [ "random", "crt" ], default = "random", help = "Mode in which to try to generate primes. Can be one of %(choices)s, defaults to %(default)s.")
parser.add_argument("-p", "--prime-type", choices = [ "2msb", "3msb" ], default = "2msb", help = "Types of primes to generate. Either the top 2 most significant bits can be set or the top 3 MSB. Can therefore be one of %(choices)s, defaults to %(default)s.")
parser.add_argument("-t", "--thread-cnt", metavar = "number", type = int, default = 8, help = "Number of threads to use. Defaults to %(default)s.")
parser.add_argument("-q", metavar = "number", type = int, default = 8, help = "Number of threads to use. Defaults to %(default)s.")
parser.add_argument("--foobar", metavar = "number", type = int, default = 8, help = "Number of threads to use. Defaults to %(default)s.")
#parser.add_argument("-b", "--bit-length", metavar = "bits", type = int, default = 2048, help = "Number of bits that generated primes will have. Defaults to %(default)s. May be imprecise in case of CRT-generated primes.")
#parser.add_argument("-n", "--num-primes", metavar = "cnt", type = int, default = 10, help = "Number of primes that are generated by primegen. Defaults to %(default)s.")
parser.add_argument("-v", "--verbose", action = "count", default = 0, help = "Increase verbosity. Can be specified multiple times.")
parser.add_argument("unix_socket", metavar = "socket", type = str, help = "UNIX socket that the KnitPi knitting server listens on.")
parser.add_argument("unix_2socket", metavar = "socket", nargs = "*", type = str, help = "UNIX socket that the KnitPi knitting server listens on.")
parser.add_argument("unix_3socket", metavar = "socket", type = str, help = "UNIX socket that the KnitPi knitting server listens on.")
