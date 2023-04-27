#!/bin/zsh
# G-CLI build script
cmake -S . -B build
make -C build

mkdir -p ~/bin
mv build/gcli ~/bin/gcli

if [ -f ~/bin/gcli ]; then
    echo "G-CLI installed successfully!"
else
    echo "G-CLI installation failed!"
fi

echo "Adding G-CLI to PATH..."

if [ "$ZSH_NAME" = "zsh" ]; then
      echo "export PATH=$PATH:~/bin" >> ~/.zshrc
else
      echo "export PATH=$PATH:~/bin" >> ~/.bashrc
fi
