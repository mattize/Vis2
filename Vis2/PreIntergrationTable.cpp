#include "PreIntegrationTable.h"

float PreIntegrationTable::volume_positions[4][4] = {
    {0.0f, 0.5f, 0.8f, 1.0f},
    {0.0f, 0.2f, 0.6f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f}
};

int PreIntegrationTable::volume_values[4][4] = {
    {0, 35, 100, 255},
    {0, 100, 35, 0},
    {0, 128, 0, 128},
    {0, 12, 40, 128}
};

float PreIntegrationTable::medium_positions[4][4] = {
    {0.0f, 0.5f, 0.8f, 1.0f},
    {0.0f, 0.2f, 0.6f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f}
};

int PreIntegrationTable::medium_values[4][4] = {
    {0, 0, 100, 255},
    {0, 100, 35, 120},
    {0, 100, 0, 0},
    {0, 0, 0, 0}
};

float PreIntegrationTable::refraction_positions[4] = {0.0f, 0.25f, 0.75f, 1.0f};

float PreIntegrationTable::refraction_values[4] = {1, 1.34, 1.45, 1.8};

void PreIntegrationTable::defineUI() {

    float currentHeight = 0;
    float fixedWidth = 230.0f; 
    ImGui::SetNextWindowSizeConstraints(ImVec2(fixedWidth, 0), ImVec2(fixedWidth, FLT_MAX));
    ImGui::SetNextWindowSize(ImVec2(fixedWidth, 0));
    ImGui::SetNextWindowPos(ImVec2(0, 0)); 

    if (ImGui::Begin("Transfer Function Editor", NULL, ImGuiWindowFlags_NoResize)) {

        if (ImGui::CollapsingHeader("Volume TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Volume " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("VolumeTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &volume_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &volume_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Medium TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Medium " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("MediumTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &medium_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &medium_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator(); 

        if (ImGui::CollapsingHeader("Refraction TF")) {
            
            ImGui::Indent();
            std::string headerLabel = "Refraction Channel";

            if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {

                    ImGui::PushItemWidth(100.0f);
                    ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &refraction_positions[point], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat(("Value##" + std::to_string(point)).c_str(), &refraction_values[point], 0.0f, 3.0f, "%.2f");

                    ImGui::PopItemWidth();
                }
            }
            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::Button("Apply TFs")) {
            applyTransportFunctions();
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset TFs")) {
            resetTransportFunctions();
        }

    }
    ImGui::End();
}

void PreIntegrationTable::applyTransportFunctions() {
    for (int i = 0; i < 4; i++) {
        std::vector<double> vol_pos;
        std::vector<double> vol_val;

        std::vector<double> med_pos;
        std::vector<double> med_val;

        std::vector<double> ref_pos;
        std::vector<double> ref_val;

        for (int j = 0; j < 4; j++) {
            vol_pos.push_back(PreIntegrationTable::volume_positions[i][j] * 255.0f);
            vol_val.push_back(PreIntegrationTable::volume_values[i][j]);

            med_pos.push_back(PreIntegrationTable::medium_positions[i][j] * 255.0f);
            med_val.push_back(PreIntegrationTable::medium_values[i][j]);

            //ref_pos.push_back(PreIntegrationTable::refraction_positions[i][j] * 255.0f);
            //ref_val.push_back(PreIntegrationTable::refraction_values[i][j]);
        }

        sortDataPoints(vol_pos, vol_val);
        sortDataPoints(med_pos, med_val);
       // sortDataPoints(ref_pos, ref_val);
        
        volume_curves[i] = tk::spline(vol_pos, vol_val);
        medium_curves[i] = tk::spline(med_pos, med_val);
        //refraction_curves[i] = tk::spline(ref_pos, ref_val);
    }

    writeSplinesToTexture(preIntegrationTables[0], volume_curves);
    //writeSplinesToTexture(preIntegrationTables[1], medium_curves);
    //writeSplinesToTexture(preIntegrationTables[2], refraction_curves);
}

void PreIntegrationTable::sortDataPoints(std::vector<double>& positions, std::vector<double>& values) {
    std::vector<std::pair<int, int>> combined;
    for (size_t i = 0; i < positions.size(); ++i) {
        combined.emplace_back(positions[i], values[i]);
    }

    std::sort(combined.begin(), combined.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (size_t i = 0; i < combined.size(); ++i) {
        positions[i] = combined[i].first;
        values[i] = combined[i].second;
    }
}

void PreIntegrationTable::setPixel(uint32_t* pixels, glm::vec4 color, int x, int y) {
    int index = 256 * x + y;
    pixels[index] = vec4ToRGBA8(color);
}

uint32_t PreIntegrationTable::vec4ToRGBA8(const glm::vec4& color) {
    uint8_t r = static_cast<uint8_t>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f);
    uint8_t g = static_cast<uint8_t>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f);
    uint8_t b = static_cast<uint8_t>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f);
    uint8_t a = static_cast<uint8_t>(glm::clamp(color.a, 0.0f, 1.0f) * 255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

void PreIntegrationTable::writeSplinesToTexture(VkImage image, std::array<tk::spline, 4> splines) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize imageSize = 256 * 256 * 4;

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = imageSize;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(device, &bufferCreateInfo, nullptr, &stagingBuffer);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vulkanDevice.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(device, &allocInfo, nullptr, &stagingBufferMemory);
    vkBindBufferMemory(device, stagingBuffer, stagingBufferMemory, 0);

    std::cout << "here" << std::endl;

    double x = 1.0;
    unsigned __int8 imageData[256 * 256 * 4];
    unsigned int nextInd = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            unsigned int ri = splines[0]((double)i);
            unsigned int rj = splines[0]((double)j);
            unsigned int gi = splines[1]((double)i);
            unsigned int gj = splines[1]((double)j);
            unsigned int bi = splines[2]((double)i);
            unsigned int bj = splines[2]((double)j);
            unsigned int ai = splines[3]((double)i);
            unsigned int aj = splines[3]((double)j);
            glm::ivec4 colorResults = (glm::ivec4(ri, gi, bi, 255) + glm::ivec4(rj, gj, bj, 255)) / 2;

            imageData[nextInd++] = (ri + rj) / 2;
            imageData[nextInd++] = (gi + gj) / 2;
            imageData[nextInd++] = (bi + bj) / 2;
            imageData[nextInd++] = (ai + aj) / 2;
        }
    }

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);

    memcpy(data, imageData, static_cast<size_t>(imageSize));

    //uint32_t* pixels = reinterpret_cast<uint32_t*>(data);

    

    vkUnmapMemory(device, stagingBufferMemory);

    VkCommandBuffer commandBuffer = vulkanDevice.beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = preIntegrationTables[0];
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageOffset = { 0, 0, 0 };
    copyRegion.imageExtent = { 256, 256, 1 };

    vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, preIntegrationTables[0], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    VkImageMemoryBarrier returnBarrier{};
    returnBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    returnBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    returnBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    returnBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    returnBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    returnBarrier.image = preIntegrationTables[0];
    returnBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    returnBarrier.subresourceRange.baseMipLevel = 0;
    returnBarrier.subresourceRange.levelCount = 1;
    returnBarrier.subresourceRange.baseArrayLayer = 0;
    returnBarrier.subresourceRange.layerCount = 1;
    returnBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    returnBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &returnBarrier
    );

    vulkanDevice.endSingleTimeCommands(commandBuffer);


    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void PreIntegrationTable::resetTransportFunctions() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            volume_positions[i][0] = 0.0f; 
            volume_positions[i][1] = 0.25f;
            volume_positions[i][2] = 0.75f;
            volume_positions[i][3] = 1.0f;
            volume_values[i][0] = 255;
            volume_values[i][1] = 128;
            volume_values[i][2] = 0; 
            volume_values[i][3] = 128;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            medium_positions[i][0] = 0.0f;
            medium_positions[i][1] = 0.25f;
            medium_positions[i][2] = 0.75f;
            medium_positions[i][3] = 1.0f;
            medium_values[i][0] = 255;
            medium_values[i][1] = 128;
            medium_values[i][2] = 0;
            medium_values[i][3] = 128;
        }
    }

    refraction_positions[0] = 0.0f;
    refraction_positions[1] = 0.25f;
    refraction_positions[2] = 0.75f;
    refraction_positions[3] = 1.0f;
    refraction_values[0] = 1;
    refraction_values[1] = 1.34;
    refraction_values[2] = 1.45;
    refraction_values[3] = 1.8;
}

void PreIntegrationTable::init() {
    for (int i = 0; i < 3; i++) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.flags = 0;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = 256;
        imageInfo.extent.height = 256;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imageInfo, nullptr, &preIntegrationTables[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, preIntegrationTables[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = vulkanDevice.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT /*properties..*/);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &tableMemories[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate table memory!");
        }

        vkBindImageMemory(device, preIntegrationTables[i], tableMemories[i], 0);

        transitionImageLayout(preIntegrationTables[i], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        VkImageViewCreateInfo view{};
        view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view.format = VK_FORMAT_R8G8B8A8_UNORM;
        view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view.subresourceRange.baseMipLevel = 0;
        view.subresourceRange.baseArrayLayer = 0;
        view.subresourceRange.layerCount = 1;
        view.subresourceRange.levelCount = 1;
        view.image = preIntegrationTables[i];

        if (vkCreateImageView(device, &view, nullptr, &tableViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create table view!");
        }
    }

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = static_cast<float>(1);
    samplerCreateInfo.maxAnisotropy = 4.0;
    samplerCreateInfo.anisotropyEnable = VK_TRUE;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (vkCreateSampler(device, &samplerCreateInfo, nullptr, &tableSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create table sampler!");
    }

    applyTransportFunctions();
}

void PreIntegrationTable::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = vulkanDevice.beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vulkanDevice.endSingleTimeCommands(commandBuffer);
}

void PreIntegrationTable::cleanup() {
    vkDestroySampler(device, tableSampler, nullptr);

    for (size_t i = 0; i < 3; i++) {
        vkDestroyImage(device, preIntegrationTables[i], nullptr);
        vkDestroyImageView(device, tableViews[i], nullptr);
        vkFreeMemory(device, tableMemories[i], nullptr);
    }
}