# Install kernel model
sudo insmod read_write.ko 
sudo lsmod read_write.ko

# Remove the kernel model 
sudo rmmod read_write

# read the model

sudo echo "this is test" > /dev/dummydriver 

# write the model 

sudo cat /dev/dummydriver 
