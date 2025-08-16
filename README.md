# OSGE - Open Source Game Engine
Download YouTube videos for free! <br/>
This Python program can run on Windows, Linux, MacOS and even Android!
> **Warning:** VPNs can provoke some unexpected results such as download failures or major slow downs.

# 📦 Requirements
- CMake compiler (**gcc/g++ recommended**).
- GLFW C++ library.
- Vulkan C++ library.
- GLM C++ library.
- STB C++ library.
- GTK C++ library (**for Linux builds**).

# 📥 Installation
1) Download the program. <br/>
2) Run the program using `python main.py` <br/>
3) Enjoy!

# ⚙️ Configuration
You can configure the program using the `config.json` file. <br/>
The default configuration is available at the bottom of this section. <br/>
Here are the different options you have:
- `"max_download_retries"` -> Maximum amount of retries the program does when a download fail, before aborting.
- `"pytube_range_size_bytes"` -> Amount in bytes to download to trigger the download callback.
- `"default_download_destination"` -> Destination path where the program puts the downloaded file by default.
- `"default_download_resolution"` -> Resolution of the video files by default (if available, otherwise we propose the highest).
- `"block_age_restricted_content"` -> Disallow the download of YouTube videos that have age restriction.
``` json
{
    "max_download_retries": 10,
    "pytube_range_size_bytes": 1048576,
    "default_download_destination": "./",
    "default_download_resolution": "1080p",
    "block_age_restricted_content": false
}
```

# 🤝 User Agreement
By downloading and/or using this program, you confirm that you are solely responsible for how you use this software, especially if you download copyrighted content or if you break the YouTube terms of service. You agree as well that this agreement extends to any prior version of the program, and any new version of the user agreement in any future update, overwrites this one.
