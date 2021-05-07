#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <sys/stat.h>
#include <map>
#include <unistd.h>


#define WIDTH 800
#define HEIGHT 600

typedef struct AppData {
    TTF_Font *font;
    SDL_Texture *dir;
    SDL_Texture *exe;
    SDL_Texture *image;
    SDL_Texture *video;
    SDL_Texture *code;
    SDL_Texture *other;
    SDL_Texture *textString;
    SDL_Texture *open;
    SDL_Texture *up;
    SDL_Texture *down;
    std::map<std::string, std::string> map;
    std::map<std::string, std::string> fileMap;
    bool open_flag = false;
    int position = 0;

} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr, std::string dir);
int listDirectory(std::string dirname, int loop, SDL_Rect startRect, SDL_Renderer *renderer, AppData *data_ptr);
void splitString(std::string text, char d, char **result);
void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size);

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    printf("HOME: %s\n", home);

    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // create window and renderer
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // initialize and perform rendering loop
    AppData data;
    
    std::string dirname(home);
    //std::string dirname = "/home/duke/Desktop";
    initialize(renderer, &data);
    render(renderer, &data, dirname);
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN: // click
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int real_y_key = floor((double)event.button.y / 25.0) * 25;
                    //printf("%d\n", real_y_key);
                    size_t pos = 0;
                    std::string delimiter = "|";
                    std::string value;
                    //printf("%s\n", data.map["0"].c_str());
                    if (event.button.y >= 15 && event.button.y <= 40 && event.button.x >= 750 && event.button.x <= 775) {
                        //printf("It's here!!!\n");
                        if (data.open_flag) {
                            data.open_flag = false;
                            int tempNum = 0 - (data.position * 250);
                            value = data.map[std::to_string(tempNum)];
                            pos = value.find(delimiter);
                            value.erase(0, pos + delimiter.length());
                            pos = value.find(delimiter);
                            value.erase(0, pos + delimiter.length());
                            std::string same_dir = value;
                            if (same_dir.back() == '.') {
                                same_dir.pop_back();
                                if (same_dir.back() == '.') {
                                    same_dir.pop_back();
                                    if (same_dir.back() == '/') {
                                        same_dir.pop_back();
                                    }
                                }
                            }
                            //printf("%s\n", same_dir.c_str());
                            data.position = 0;
                            render(renderer, &data, same_dir);
                        } else {
                            data.open_flag = true;
                            int tempNum = 0 - (data.position * 250);
                            value = data.map[std::to_string(tempNum)];
                            pos = value.find(delimiter);
                            value.erase(0, pos + delimiter.length());
                            pos = value.find(delimiter);
                            value.erase(0, pos + delimiter.length());
                            std::string same_dir = value;
                            if (same_dir.back() == '.') {
                                same_dir.pop_back();
                                if (same_dir.back() == '.') {
                                    same_dir.pop_back();
                                    if (same_dir.back() == '/') {
                                        same_dir.pop_back();
                                    }
                                }
                            }
                            render(renderer, &data, same_dir);
                        }
                    } else if (event.button.y >= 530 && event.button.y <= 555 && event.button.x >= 750 && event.button.x <= 775) {
                        // up
                        int tempNum = 0 - (data.position * 250);
                        value = data.map[std::to_string(tempNum)];
                        pos = value.find(delimiter);
                        value.erase(0, pos + delimiter.length());
                        pos = value.find(delimiter);
                        value.erase(0, pos + delimiter.length());
                        std::string same_dir = value;
                        if (same_dir.back() == '.') {
                            same_dir.pop_back();
                            if (same_dir.back() == '.') {
                                same_dir.pop_back();
                                if (same_dir.back() == '/') {
                                    same_dir.pop_back();
                                }
                            }
                        }
                        if (data.position > 0) {
                            data.position--;
                        }
                        //printf("%s\n", same_dir.c_str());
                        render(renderer, &data, same_dir);
                    } else if (event.button.y >= 560 && event.button.y <= 585 && event.button.x >= 750 && event.button.x <= 775) {
                        // down
                        int tempNum = 0 - (data.position * 250);
                        value = data.map[std::to_string(tempNum)];
                        pos = value.find(delimiter);
                        value.erase(0, pos + delimiter.length());
                        pos = value.find(delimiter);
                        value.erase(0, pos + delimiter.length());
                        std::string same_dir = value;
                        if (same_dir.back() == '.') {
                            same_dir.pop_back();
                            if (same_dir.back() == '.') {
                                same_dir.pop_back();
                                if (same_dir.back() == '/') {
                                    same_dir.pop_back();
                                }
                            }
                        }
                        data.position++;
                        //printf("%s\n", same_dir.c_str());
                        render(renderer, &data, same_dir);
                    } else if (!data.map[std::to_string(real_y_key).c_str()].empty()) {
                        value = data.map[std::to_string(real_y_key).c_str()];
                        pos = value.find(delimiter);
                        std::string start_x = value.substr(0, pos); // start x
                        value.erase(0, pos + delimiter.length());
                        pos = value.find(delimiter);
                        std::string end_x = value.substr(0, pos); // end x
                        value.erase(0, pos + delimiter.length());
                        //printf("%s\n", value.c_str());
                        //printf("%s\n", start_x.c_str());
                        int startX = std::stoi(start_x);
                        int endX = std::stoi(end_x);
                        if (real_y_key <= 600 && event.button.x >= startX && event.button.x <= endX) {
                            //printf("%s , %s\n", std::to_string(real_y_key).c_str(), data.map[std::to_string(real_y_key).c_str()].c_str());
                            //printf("It's working!!!\n");
                            data.position = 0;
                            render(renderer, &data, value);
                        }
                    } else if (!data.fileMap[std::to_string(real_y_key).c_str()].empty()) {
                        value = data.fileMap[std::to_string(real_y_key).c_str()];
                        pos = value.find(delimiter);
                        std::string start_x = value.substr(0, pos); // start x
                        value.erase(0, pos + delimiter.length());
                        pos = value.find(delimiter);
                        std::string end_x = value.substr(0, pos); // end x
                        value.erase(0, pos + delimiter.length());
                        //printf("%s\n", value.c_str());
                        //printf("%s\n", start_x.c_str());
                        int startX = std::stoi(start_x);
                        int endX = std::stoi(end_x);
                        if (real_y_key <= 600 && event.button.x >= startX && event.button.x <= endX) {
                            // fork
                            char **cmd_2;
                            std::string tp = "/usr/bin/xdg-open";
                            value = "xdg-open " + value;
                            allocateArrayOfCharArrays(&cmd_2, 3, 64);
                            splitString(value,' ',cmd_2);
                            //printf("%s\n", value.c_str());
                            int pid;
                            pid = fork();
                            if (pid == 0) { // child
                                execv(tp.c_str(), cmd_2);
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION: // move
                
                //render(renderer, &data);
                break;
        }
        
    }

    // clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();

    return 0;
}

void initialize(SDL_Renderer *renderer, AppData *data_ptr)
{
    SDL_Surface *codeSur = IMG_Load("resrc/images/code.png");
    data_ptr->code = SDL_CreateTextureFromSurface(renderer, codeSur);
    SDL_FreeSurface(codeSur);

    SDL_Surface *imageSur = IMG_Load("resrc/images/image.png");
    data_ptr->image = SDL_CreateTextureFromSurface(renderer, imageSur);
    SDL_FreeSurface(imageSur);

    SDL_Surface *dirSur = IMG_Load("resrc/images/directory.png");
    data_ptr->dir = SDL_CreateTextureFromSurface(renderer, dirSur);
    SDL_FreeSurface(dirSur);

    SDL_Surface *exeSur = IMG_Load("resrc/images/exe.png");
    data_ptr->exe = SDL_CreateTextureFromSurface(renderer, exeSur);
    SDL_FreeSurface(exeSur);

    SDL_Surface *otherSur = IMG_Load("resrc/images/others.png");
    data_ptr->other = SDL_CreateTextureFromSurface(renderer, otherSur);
    SDL_FreeSurface(otherSur);

    SDL_Surface *videoSur = IMG_Load("resrc/images/video.png");
    data_ptr->video = SDL_CreateTextureFromSurface(renderer, videoSur);
    SDL_FreeSurface(videoSur);

    SDL_Surface *openSur = IMG_Load("resrc/images/open.png");
    data_ptr->open = SDL_CreateTextureFromSurface(renderer, openSur);
    SDL_FreeSurface(openSur);

    SDL_Surface *upSur = IMG_Load("resrc/images/up.png");
    data_ptr->up = SDL_CreateTextureFromSurface(renderer, upSur);
    SDL_FreeSurface(upSur);

    SDL_Surface *downSur = IMG_Load("resrc/images/down.png");
    data_ptr->down = SDL_CreateTextureFromSurface(renderer, downSur);
    SDL_FreeSurface(downSur);

    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 18);
    
}

void render(SDL_Renderer *renderer, AppData *data_ptr, std::string dir)
{
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // erase renderer content
    SDL_RenderClear(renderer);
    data_ptr->map.clear();
    data_ptr->fileMap.clear();

    SDL_Rect openRect;
    openRect.x = 750;
    openRect.y = 15;
    openRect.w = 25;
    openRect.h = 25;
    SDL_RenderCopy(renderer, data_ptr->open, NULL, &openRect); // open recursive

    SDL_Rect upRect;
    upRect.x = 750;
    upRect.y = 530;
    upRect.w = 25;
    upRect.h = 25;
    SDL_RenderCopy(renderer, data_ptr->up, NULL, &upRect); // up button

    SDL_Rect downRect;
    downRect.x = 750;
    downRect.y = 560;
    downRect.w = 25;
    downRect.h = 25;
    SDL_RenderCopy(renderer, data_ptr->down, NULL, &downRect); // down button

    SDL_Rect startRect;
    startRect.x = 40;
    startRect.y = 0 - (data_ptr->position * 250);
    //printf("%d\n", startRect.y);
    startRect.w = 20;
    startRect.h = 20;
    listDirectory(dir, 0, startRect, renderer, data_ptr);
    // show rendered frame
    SDL_RenderPresent(renderer);
}

int listDirectory(std::string dirname, int loop, SDL_Rect startRect, SDL_Renderer *renderer, AppData *data_ptr)
{
    struct stat info;

    int err = stat(dirname.c_str(), &info);
    if (err == 0 && S_ISDIR(info.st_mode)) { // is dir
        std::vector<std::string> file_list;
        DIR* dir = opendir(dirname.c_str());
        
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            file_list.push_back(entry->d_name);
        }
        closedir(dir);
        // alphabetical order
        std::sort(file_list.begin(), file_list.end());

        struct stat file_info;
        int file_err;
        //SDL_Rect textRectNew = startRect;
        
        for (int i = 0; i < file_list.size(); i++) {

            if (file_list[i] != ".") {
            //if (file_list[i].substr(0, 1) != "." || file_list[i] == "..") { // skip folders start with '.', those killed my VM 6 times!!
                std::string full_path = dirname + "/" + file_list[i];
                file_err = stat(full_path.c_str(), &file_info);
                if (file_err) {
                    fprintf(stderr, "Uh oh! Should not get here\n");
                } else if (S_ISDIR(file_info.st_mode)) {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->dir, NULL, &startRect); // icon
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};

                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *dir_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *dir_string= SDL_CreateTextureFromSurface(renderer, dir_surf);
                        SDL_FreeSurface(dir_surf);
                        SDL_QueryTexture(dir_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, dir_string, NULL, &startRect); // file name
                    }
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->map[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;
                    //printf("%s , %s\n", key.c_str(), data_ptr->map[key].c_str());
                    
                    //printf("%s\n", data_ptr->map["0"].c_str());
                    startRect.x = 600;
                    startRect.w = 20;
                    startRect.h = 20;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    // only render when needed
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                    if (file_list[i] != "." && file_list[i] != ".." && data_ptr->open_flag) {
                        startRect.y = listDirectory(full_path, loop+1, startRect, renderer, data_ptr);
                    }
                    
                } else if ((S_IEXEC & file_info.st_mode) != 0) {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->exe, NULL, &startRect);
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                        SDL_FreeSurface(file_surf);
                        SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    }
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->fileMap[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;

                    startRect.x = 500;
                    startRect.w = 20;
                    startRect.h = 20;
                    int temp;
                    std::string file_size;
                    if (file_info.st_size >= 10732109824) {
                        temp = file_info.st_size / 10732109824;
                        file_size = std::to_string(temp) + " GB";
                    } else if (file_info.st_size >= 10480576) {
                        temp = file_info.st_size / 10480576;
                        file_size = std::to_string(temp) + " MB";
                    } else if (file_info.st_size >= 1024) {
                        temp = file_info.st_size / 1024;
                        file_size = std::to_string(temp) + " KB";
                    } else {
                        temp = file_info.st_size;
                        file_size = std::to_string(temp) + " B";
                    }
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                        SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                        SDL_FreeSurface(size_surf);
                        SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, size_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.x = 600;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpeg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "png" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tif" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tiff" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "gif") {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->image, NULL, &startRect); // icon
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                        SDL_FreeSurface(file_surf);
                        SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, file_string, NULL, &startRect); // text
                    }
                    
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->fileMap[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;

                    startRect.x = 500;
                    startRect.w = 20;
                    startRect.h = 20;
                    int temp;
                    std::string file_size;
                    if (file_info.st_size >= 10732109824) {
                        temp = file_info.st_size / 10732109824;
                        file_size = std::to_string(temp) + " GB";
                    } else if (file_info.st_size >= 10480576) {
                        temp = file_info.st_size / 10480576;
                        file_size = std::to_string(temp) + " MB";
                    } else if (file_info.st_size >= 1024) {
                        temp = file_info.st_size / 1024;
                        file_size = std::to_string(temp) + " KB";
                    } else {
                        temp = file_info.st_size;
                        file_size = std::to_string(temp) + " B";
                    }
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                        SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                        SDL_FreeSurface(size_surf);
                        SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, size_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.x = 600;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mp4" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mov" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mkv" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "avi" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "webm") {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->video, NULL, &startRect);
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                        SDL_FreeSurface(file_surf);
                        SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    }
                    
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->fileMap[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;

                    startRect.x = 500;
                    startRect.w = 20;
                    startRect.h = 20;
                    int temp;
                    std::string file_size;
                    if (file_info.st_size >= 10732109824) {
                        temp = file_info.st_size / 10732109824;
                        file_size = std::to_string(temp) + " GB";
                    } else if (file_info.st_size >= 10480576) {
                        temp = file_info.st_size / 10480576;
                        file_size = std::to_string(temp) + " MB";
                    } else if (file_info.st_size >= 1024) {
                        temp = file_info.st_size / 1024;
                        file_size = std::to_string(temp) + " KB";
                    } else {
                        temp = file_info.st_size;
                        file_size = std::to_string(temp) + " B";
                    }
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                        SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                        SDL_FreeSurface(size_surf);
                        SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, size_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.x = 600;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "h" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "c" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "cpp" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "py" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "java" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "js") {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->code, NULL, &startRect);
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                        SDL_FreeSurface(file_surf);
                        SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    }
                    
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->fileMap[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;

                    startRect.x = 500;
                    startRect.w = 20;
                    startRect.h = 20;
                    int temp;
                    std::string file_size;
                    if (file_info.st_size >= 10732109824) {
                        temp = file_info.st_size / 10732109824;
                        file_size = std::to_string(temp) + " GB";
                    } else if (file_info.st_size >= 10480576) {
                        temp = file_info.st_size / 10480576;
                        file_size = std::to_string(temp) + " MB";
                    } else if (file_info.st_size >= 1024) {
                        temp = file_info.st_size / 1024;
                        file_size = std::to_string(temp) + " KB";
                    } else {
                        temp = file_info.st_size;
                        file_size = std::to_string(temp) + " B";
                    }
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                        SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                        SDL_FreeSurface(size_surf);
                        SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, size_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.x = 600;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else {
                    startRect.x += 20 * loop;
                    int start_x = startRect.x; // x after indent
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_RenderCopy(renderer, data_ptr->other, NULL, &startRect);
                    }
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                        SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                        SDL_FreeSurface(file_surf);
                        SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    }
                    
                    int old_x = startRect.x;
                    int end_x = old_x + startRect.w;
                    std::string key = std::to_string(startRect.y);
                    data_ptr->fileMap[key] = std::to_string(start_x) + "|" + std::to_string(end_x) + "|" + full_path;

                    startRect.x = 500;
                    startRect.w = 20;
                    startRect.h = 20;
                    int temp;
                    std::string file_size;
                    if (file_info.st_size >= 10732109824) {
                        temp = file_info.st_size / 10732109824;
                        file_size = std::to_string(temp) + " GB";
                    } else if (file_info.st_size >= 10480576) {
                        temp = file_info.st_size / 10480576;
                        file_size = std::to_string(temp) + " MB";
                    } else if (file_info.st_size >= 1024) {
                        temp = file_info.st_size / 1024;
                        file_size = std::to_string(temp) + " KB";
                    } else {
                        temp = file_info.st_size;
                        file_size = std::to_string(temp) + " B";
                    }
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                        SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                        SDL_FreeSurface(size_surf);
                        SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, size_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.x = 600;
                    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
                    modeval[0] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                    modeval[1] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                    modeval[2] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
                    modeval[3] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                    modeval[4] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                    modeval[5] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
                    modeval[6] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                    modeval[7] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                    modeval[8] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
                    modeval[9] = '\0';
                    if (startRect.y <= 600 && startRect.y >= 0) {
                        SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                        SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                        SDL_FreeSurface(permit_surf);
                        SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                        SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    }
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                }
                SDL_RenderPresent(renderer);
                
            }
            
        }
        return startRect.y;
    } else {
        fprintf(stderr, "Error: directory '%s' not found\n", dirname.c_str());
        return 0;
    }
}

void splitString(std::string text, char d, char **result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::vector<std::string> list;
    std::string token;
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    list.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    list.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        list.push_back(token);
    }

    for (i = 0; i < list.size(); i++)
    {
        strcpy(result[i], list[i].c_str());
    }
    result[list.size()] = NULL;
}

void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size)
{
    int i;
    *array_ptr = new char*[array_length];
    for (i = 0; i < array_length; i++)
    {
        (*array_ptr)[i] = new char[item_size];
    }
}
