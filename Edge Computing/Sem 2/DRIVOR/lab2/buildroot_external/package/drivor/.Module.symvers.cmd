cmd_/home/debian/buildroot_external/package/drivor/Module.symvers :=  sed 's/ko$$/o/'  /home/debian/buildroot_external/package/drivor/modules.order | scripts/mod/modpost -m      -o /home/debian/buildroot_external/package/drivor/Module.symvers -e -i Module.symvers -T - 
