#include <iostream>
#include <string>
#include <curl/curl.h> // You'll need to install libcurl for HTTP requests

// Your Merriam-Webster API key
const std::string API_KEY = "YOUR_API_KEY_HERE"; // Replace with your actual API key

// Function to make an HTTP GET request to the Merriam-Webster API
std::string makeHttpRequest(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* buffer, size_t size, size_t count, void* stream) -> size_t {
            ((std::string*)stream)->append((char*)buffer, 0, size * count);
            return size * count;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
    }

    return response;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <word>" << std::endl;
        return 1;
    }

    std::string word(argv[1]);

    // Construct the URL for the Merriam-Webster API request
    std::string url = "https://www.dictionaryapi.com/api/v3/references/collegiate/json/" + word + "?key=" + API_KEY;

    // Make the HTTP GET request
    std::string response = makeHttpRequest(url);

    if (!response.empty()) {
        // Print the definition
        std::cout << "Definition of '" << word << "':" << std::endl;
        std::cout << response << std::endl;
    } else {
        std::cerr << "Failed to retrieve the definition for '" << word << "'" << std::endl;
        return 1;
    }

    return 0;
}
