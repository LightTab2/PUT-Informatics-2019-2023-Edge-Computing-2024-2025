cmd_/home/debian/buildroot_external/package/drivor/drivor.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T arch/x86/module.lds -o /home/debian/buildroot_external/package/drivor/drivor.ko /home/debian/buildroot_external/package/drivor/drivor.o /home/debian/buildroot_external/package/drivor/drivor.mod.o;  true