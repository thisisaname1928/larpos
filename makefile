LIMINE_BIN=limine-binary/limine
ACHROIT_BIN=achroit/achroit
SAMPLE_IMAGE=sample.img
SAMPLE_IMAGE_SIZE=50

run: buildRootImg
	@qemu-system-x86_64 $(SAMPLE_IMAGE) -m 1G -enable-kvm

buildRootImg: $(SAMPLE_IMAGE) achroitBuild
	@mkdir -p iso_root
	@mkdir -p iso_root/boot
	@cp limine-binary/limine-bios.sys iso_root/boot/
	@cp limine.conf iso_root/boot
	@cp kernel/larpkernel iso_root/boot
	@mcopy -soi $(SAMPLE_IMAGE)@@1M iso_root/* ::

achroitBuild:
	@$(ACHROIT_BIN) .

$(SAMPLE_IMAGE):
	@echo "Creating sample image..."
	@dd if=/dev/zero of=$(SAMPLE_IMAGE) bs=1M count=$(SAMPLE_IMAGE_SIZE)
	@/sbin/parted -s $(SAMPLE_IMAGE) mklabel msdos
	@/sbin/parted -s $(SAMPLE_IMAGE) mkpart primary fat32 1MiB 100%
	@/sbin/parted -s $(SAMPLE_IMAGE) set 1 boot on
	@mformat -i $(SAMPLE_IMAGE)@@1M -F
	@$(LIMINE_BIN) bios-install $(SAMPLE_IMAGE)

$(ACHROIT_BIN):
	@echo "Building achroit..."
	@cd achroit && make

$(LIMINE_BIN): limine.tar.xz
	@echo "Decompressing $<"
	@tar -xf $<
	@echo "Building limine"
	@cd limine-binary && make
	@echo "ok"

limine.tar.xz:
	@echo "Download limine..."
	@curl -L https://github.com/Limine-Bootloader/Limine/releases/download/v12.3.1/limine-binary.tar.xz --output limine.tar.xz

clean:
	@rm -f $(SAMPLE_IMAGE)
	@cd kernel && make clean