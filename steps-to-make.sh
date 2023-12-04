brew install pcre
wget http://www.iausofa.org/2023_1011_C/sofa_c-20231011.tar.gz
wget ftp://ftp.astro.caltech.edu/pub/pgplot/pgplot5.2.tar.gz

tar -xvzf sofa_c-20231011.tar.gz

brew install automake
brew install libtool

#This makes the libsofa_c.a we will need later
cd sofa/VERSION/c/src
make
# on mac
sudo mkdir /usr/local/lib/sofa
cp *.o *.a *.h /usr/local/lib/sofa/
# I think the sofa bit would be better placed in the usr/src directory
# on scrtpc
make test

# Make libcpgplot.a
# On mac, do this
http://mingus.as.arizona.edu/~bjw/software/pgplot_fix.html
# Otherwise follow the instructions in the README for PGPLOT

brew install pkg-config
pkg-config --libs --cflags /opt/homebrew/Cellar/pcre/*/lib/pkgconfig/libpcrecpp.pc 

# on mac, do this
export CPPFLAGS="-I/opt/homebrew/Cellar/pcre/8.45/include -I/usr/local/lib/pgplot -I/usr/local/lib/sofa"
export LDFLAGS="-L/opt/homebrew/Cellar/pcre/8.45/lib -L/usr/local/lib/pgplot -L/usr/local/lib/sofa"
# on scrtpc, do this
export CPPFLAGS="-I/home/csc/pfstcb/include/"
export LDFLAGS="-L/home/csc/pfstcb/lib/"

./configure



# On SCRTC, do this

module load GCCcore/11.3.0 Autotools/20220317 libtool/2.4.7 PCRE/8.45 PGPLOT/5.2.2

wget http://www.iausofa.org/2023_1011_C/sofa_c-20231011.tar.gz

cd sofa/VERSION/c/src
make
make test

# on scrtpc, do this
export CPPFLAGS="-I$HOME/include/"
export LDFLAGS="-L$HOME/lib/"

./bootstrap

./configure

make

make install