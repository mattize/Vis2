#pragma once

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Camera.h"
#include "Light.h"
#include "Texture.h"

#include <fstream>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;


    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

const int MAX_FRAMES_IN_FLIGHT = 2;


struct Vertex {
    glm::vec3 pos;
    glm::vec2 tex;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, tex);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && tex == other.tex;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec2>()(vertex.tex) << 1)) >> 1);
        }
    };
}

struct CameraUniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct AlgoUniformBufferObject {
    alignas(16) glm::mat4 inverseViewMatrix;   
    alignas(16) glm::mat4 viewMatrix;          
    alignas(16) glm::vec3 lightVSPos;          
    alignas(16) glm::vec3 lightColor;          
    alignas(4)  float planeDistance;           
    alignas(16) glm::vec3 middleOfPlaneVS;     
    alignas(4)  float sphereRadius;            
    alignas(8)  glm::vec2 planeSides;          
    alignas(8)  glm::ivec2 dims;               
    alignas(16) glm::vec3 refractionPos;       
    alignas(16) glm::vec4 refractionValue;     
    alignas(4)  float voxelDepth;              
    alignas(4)  float planeWidth;              
    alignas(4)  float planeHeight;     
};

struct PerPlanePushConstant {
    int layer;
    float currentZVS;
};

class VulkanHandler {
public:
    VulkanHandler() {};
    VulkanHandler(GLFWwindow* window) : window(window) {};

    void initVulkan();
    void drawFrame(int numPlanes, glm::vec3 middleOfPlaneVS, float planeDistance);
    void cleanup();
    void setDeviceWaitIdle();
    void createCube();
    void createQuad(Texture texture);
    void updateMVP(float dt, Camera& camera);
    void updateAlgo(glm::mat4 inverseViewMatrix, glm::mat4 viewMatrix, float planeDistance, glm::vec3 middleOfPlaneVS, float sphereRadius, glm::vec2 planeSides,
        glm::ivec2 dims, glm::vec3 refractionPos, glm::vec4 refractionValue, float voxelDepth, float planeWidth, float planeHeight, Light light);
    void dispatchCompute(int width, int heigth, int depth);
    void runAlgo(int numPlanes, glm::vec3 middleOfPlaneVS, float planeDistance);

    VkDevice& getDevice();
    VkPhysicalDevice& getPhysicalDevice();
    VkRenderPass& getRenderPass();
    uint32_t getCurrentFrame();
    VkCommandPool& getCommandPool();
    VkQueue& getGraphicsQueue();

private:
    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue computeQueue;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    std::vector<VkFramebuffer> algoFramebuffers;

    VkRenderPass renderPass;
    VkRenderPass algoRenderPass;

    VkCommandPool commandPool;

    uint32_t imageIndex;
        
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkCommandBuffer> computeCommandBuffers;
    std::vector<VkCommandBuffer> algoCommandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> computeInFlightFences;
    std::vector<VkSemaphore> computeFinishedSemaphores;
    std::vector<VkFence> algoInFlightFences;
    std::vector<VkSemaphore> algoFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkDescriptorSetLayout algoDescriptorSetLayout;

    VkPipelineLayout renderPipelineLayout;
    VkPipelineLayout algoPipelineLayout;
    VkPipelineLayout computePipelineLayout;
    VkPipeline computePipeline;
    VkPipeline renderPipeline;
    VkPipeline algoPipeline;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> camUniformBuffers;
    std::vector<VkDeviceMemory> camUniformBuffersMemory;
    std::vector<void*> camUniformBuffersMapped;
    std::vector<VkBuffer> algoUniformBuffers;
    std::vector<VkDeviceMemory> algoUniformBuffersMemory;
    std::vector<void*> algoUniformBuffersMapped;

    VkDescriptorPool computeDescriptorPool;
    std::vector<VkDescriptorSet> computeDescriptorSets;
    VkDescriptorPool algoDescriptorPool;
    std::vector<VkDescriptorSet> algoDescriptorSets;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    bool framebufferResized = false;

    std::vector<std::string> bufferNames{ "vpb", "vdb", "lb", "ldb", "cb", "mb"};
    std::vector<VkImage> buffers;
    std::vector<VkDeviceMemory> bufferMemories;
    std::vector<VkImageView> bufferViews;
    VkSampler bufferSampler;

    std::vector<Vertex> cube_vertices = {
        // positions        // texture Coords
        { {-1.0f, -1.0f, 1.0f}, {1.0f, 1.0f} },
        { {1.0f, -1.0f, 1.0f }, {1.0f, 1.0f} },
        { {1.0f, 1.0f, 1.0f }, {1.0f, 0.0f} },
        { {-1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} },

        { {-1.0f, -1.0f, -1.0f }, {0.0f, 0.0f} },
        { {1.0f, -1.0f, -1.0f }, {1.0f, 0.0f} },
        { {1.0f, 1.0f, -1.0f }, {0.0f, 1.0f} },
        { {-1.0f, 1.0f, -1.0f }, {0.0f, 0.0f} }
    };


    std::vector<uint32_t> cube_indices = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3
    };

    std::vector<Vertex> quad_vertices = {
        // positions        // texture Coords
        { {-1.0f,  1.0f, 0.0f}, {0.0f,1.0f} },
        { {-1.0f, -1.0f, 0.0f}, {0.0f,0.0f} },
        { {1.0f,  1.0f, 0.0f}, {1.0f,1.0f} },
        { {1.0f, -1.0f, 0.0f}, {1.0f,0.0f} }
    };

    std::vector<uint32_t> quad_indices = {
        0, 1, 2,
        1, 3, 2
    };
    

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void setFramebufferResized();

    void cleanupSwapChain();

    void recreateSwapChain();

    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createComputeCommandBuffers();

    void createSwapChain();

    void transitionImageForWrite(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout newLayout);

    void transitionImageForSampling(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout newLayout);

    void createImageViews();

    void createRenderPass();

    void createFramebuffers();

    void createAlgoDescriptorSetLayout();

    void createDescriptorSetLayout();

    void createStorageImages(uint32_t width, uint32_t height, VkFormat format);

    void createCommandPool();

    void createDepthResources();

    void recordAlgoCommandBuffer(VkCommandBuffer commandBuffer, int numPlanes, glm::vec3 middleOfPlaneVS, float planeDistance);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void recordComputeCommandBuffer(VkCommandBuffer commandBuffer);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void createCommandBuffers();

    void createAlgoCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, int numPlanes, glm::vec3 middleOfPlaneVS, float planeDistance);

    void createSyncObjects();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    void createRenderPipeline();

    void createAlgoPipeline();

    void createComputePipeline();

    void createComputeDescriptorSetLayout();

    std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createVertexBuffer(std::vector<Vertex> vertices);

    void createIndexBuffer(std::vector<uint32_t> indices);

    void createAlgoFramebuffers();

    void createUniformBuffers();

    void createAlgoDescriptorPool();

    void createComputeDescriptorPool();

    void createDescriptorPool();

    void createAlgoRenderPass();

    void createComputeDescriptorSets(Texture texture);

    void createAlgoDescriptorSets(Texture texture);

    void createDescriptorSets(Texture texture);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void textureBarrier(VkCommandBuffer commandBuffer, VkImage image);
};