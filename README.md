# Installation on Gentoo

1. Add savedconfig USE flag
```bash
# echo "x11-wm/dwm savedconfig" > /etc/portage/package.use/dwm

```

2. Create diff file and move it to the patches directory
```bash
$ git diff cfb8627a80a334f200f68c2c8f3e384313ebbaf5..HEAD -- . ':(exclude)config.def.h' ':(exclude)README.md' ':(exclude).dwm' > mypatch.diff

# mkdir -p /etc/portage/patches/x11-wm/dwm

# cp mypatch.diff /etc/portage/patches/x11-wm/dwm
```

3. Copy the configuration file to savedconfig
```bash
# mkdir -p /etc/portage/savedconfig/x11-wm

# cp config.def.h /etc/portage/savedconfig/x11-wm/dwm
# ln -s /etc/portage/savedconfig/x11-wm/dwm /etc/portage/savedconfig/x11-wm/dwm.h
```

4. Install dwm
```bash
# emerge dwm
```
