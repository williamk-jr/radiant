if [[ ! -v VK_ADD_LAYER_PATH ]]; then
  source ~/sdk/VulkanSdk/setup-env.sh
fi

# Defer script to python
python3 ./scripts/python/run.py
