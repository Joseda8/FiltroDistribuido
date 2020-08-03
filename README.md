ssh-keygen -t rsa 
ssh mpiuser@client1 mkdir -p .ssh
sudo ssh-copy-id -i .ssh/id_rsa.pub mpiuser@client1
sudo mount -t nfs master:/home/mpiuser/storage ~/storage

convert -crop 50%x100% scorpion.jpeg out.jpeg
convert out-0.jpeg out-1.jpeg +append result.jpg
