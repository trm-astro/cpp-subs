brew install pcre
wget http://www.iausofa.org/2023_1011_C/sofa_c-20231011.tar.gz
wget ftp://ftp.astro.caltech.edu/pub/pgplot/pgplot5.2.tar.gz

tar -xvzf sofa_c-20231011.tar.gz

brew install automake
brew install libtool

#This makes the libsofa_c.a we will need later
cd sofa/VERSION/c/src
make
sudo mkdir /usr/local/lib/sofa
cp *.o *.a *.h /usr/local/lib/sofa/
# I think the sofa bit would be better placed in the usr/src directory


# Make libcpgplot.a
# On mac, do this
http://mingus.as.arizona.edu/~bjw/software/pgplot_fix.html
# Otherwise follow the instructions in the README for PGPLOT

brew install pkg-config
pkg-config --libs --cflags /opt/homebrew/Cellar/pcre/*/lib/pkgconfig/libpcrecpp.pc 

export CPPFLAGS="-I/opt/homebrew/Cellar/pcre/8.45/include -I/usr/local/lib/pgplot -I/usr/local/lib/sofa"
export LDFLAGS="-L/opt/homebrew/Cellar/pcre/8.45/lib -L/usr/local/lib/pgplot -L/usr/local/lib/sofa"


./configure