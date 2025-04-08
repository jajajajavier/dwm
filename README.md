# Installation on Gentoo

1. Enable the savedconfig USE flag
```bash
sudo echo "x11-wm/dwm savedconfig" > /etc/portage/package.use/dwm

```

2. Create a diff file and move it in the patches directory
```bash
git diff cfb8627a80a334f200f68c2c8f3e384313ebbaf5..HEAD -- . ':(exclude)config.def.h' ':(exclude)README.md' ':(exclude).dwm' > mypatch.diff

sudo mkdir -p /etc/portage/patches/x11-wm/dwm

sudo cp mypatch.diff /etc/portage/patches/x11-wm/dwm
```

3. Copy the configuration file to the savedconfig location
```bash
sudo mkdir -p /etc/portage/savedconfig/x11-wm

sudo cp config.def.h /etc/portage/savedconfig/x11-wm/dwm
sudo ln -s /etc/portage/savedconfig/x11-wm/dwm /etc/portage/savedconfig/x11-wm/dwm.h
```

4. Install dwm
```bash
sudo emerge dwm
```
