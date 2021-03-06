
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386


objects = loader.o gdt.o driver.o port.o interruptstubs.o interrupts.o keyboard.o kernel.o



%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

uranium.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

test.iso: uranium.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp uranium.bin iso/boot/uranium.bin
	echo 'set timeout=5'					  > iso/boot/grub/grub.cfg
	echo 'set default=0'					 >> iso/boot/grub/grub.cfg
	echo '' 								 >> iso/boot/grub/grub.cfg
	echo 'menuentry "Uranium Test ISO" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/uranium.bin'	>> iso/boot/grub/grub.cfg
	echo '	boot'							 >> iso/boot/grub/grub.cfg
	echo '}'								 >> iso/boot/grub/grub.cfg
	
	echo 'menuentry "First HDD {' >> iso/boot/grub/grub.cfg
	echo '	set root=(hd0)'	>> iso/boot/grub/grub.cfg
	echo '	chainloader+1'							 >> iso/boot/grub/grub.cfg
	echo '	boot'							 >> iso/boot/grub/grub.cfg
	echo '}'								 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=test.iso iso
	rm -rf iso

vbox: test.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'My Operating System' &

test: test.iso
	(killall qemu-system-i386 && sleep 1) || true
	qemu-system-i386 -cdrom test.iso -m 128M &

qemu: test.iso
	(killall qemu-system-i386 && sleep 1) || true
	qemu-system-i386 -cdrom test.iso -m 128M &

install: uranium.bin
	sudo cp $< /boot/uranium.bin

.PHONY: clean
clean:
	(killall qemu-system-i386 && sleep 1) || true
	(killall VirtualBox && sleep 1) || true
	rm -f $(objects) uranium.bin test.iso
