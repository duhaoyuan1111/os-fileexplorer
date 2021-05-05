#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <sys/stat.h>


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

} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
int listDirectory(std::string dirname, int loop, SDL_Rect startRect, SDL_Renderer *renderer, AppData *data_ptr);

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
    initialize(renderer, &data);
    render(renderer, &data);
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN: // click
                if (event.button.button == SDL_BUTTON_LEFT) {
                    
                }
                break;
            case SDL_MOUSEMOTION: // move
                if (event.motion.x >= 20 && event.motion.x <= 400 && event.motion.y >= 40 && event.motion.y <= 65) {
                    
                }
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

    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 18);
    
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // erase renderer content
    SDL_RenderClear(renderer);
    SDL_Rect startRect;
    startRect.x = 40;
    startRect.y = 40;
    startRect.w = 20;
    startRect.h = 20;
    char *home = getenv("HOME");
    //std::string dirname(home);
    std::string dirname = "/home/duke/Desktop";
    listDirectory(dirname, 0, startRect, renderer, data_ptr);
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
                std::string full_path = dirname + "/" + file_list[i];
                file_err = stat(full_path.c_str(), &file_info);
                if (file_err) {
                    fprintf(stderr, "Uh oh! Should not get here\n");
                } else if (S_ISDIR(file_info.st_mode)) {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->dir, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *dir_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *dir_string= SDL_CreateTextureFromSurface(renderer, dir_surf);
                    SDL_FreeSurface(dir_surf);
                    SDL_QueryTexture(dir_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, dir_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                    if (file_list[i] != "." && file_list[i] != "..") {
                        startRect.y = listDirectory(full_path, loop+1, startRect, renderer, data_ptr);
                    }
                    
                } else if ((S_IEXEC & file_info.st_mode) != 0) {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->exe, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
                    startRect.x = 500;
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
                    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                    SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                    SDL_FreeSurface(size_surf);
                    SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, size_string, NULL, &startRect);
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpeg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "png" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tif" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tiff" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "gif") {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->image, NULL, &startRect); // icon
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect); // text
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
                    startRect.x = 500;
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
                    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                    SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                    SDL_FreeSurface(size_surf);
                    SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, size_string, NULL, &startRect);
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mp4" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mov" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mkv" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "avi" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "webm") {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->video, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
                    startRect.x = 500;
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
                    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                    SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                    SDL_FreeSurface(size_surf);
                    SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, size_string, NULL, &startRect);
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "h" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "c" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "cpp" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "py" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "java" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "js") {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->code, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
                    startRect.x = 500;
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
                    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                    SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                    SDL_FreeSurface(size_surf);
                    SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, size_string, NULL, &startRect);
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x = old_x;
                    startRect.x -= 30;
                    startRect.x -= 20 * loop;
                } else {
                    startRect.x += 20 * loop;
                    SDL_RenderCopy(renderer, data_ptr->other, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    int old_x = startRect.x;
                    startRect.x = 500;
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
                    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, file_size.c_str(), color);
                    SDL_Texture *size_string= SDL_CreateTextureFromSurface(renderer, size_surf);
                    SDL_FreeSurface(size_surf);
                    SDL_QueryTexture(size_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, size_string, NULL, &startRect);
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
                    SDL_Surface *permit_surf = TTF_RenderText_Solid(data_ptr->font, modeval, color);
                    SDL_Texture *permit_string= SDL_CreateTextureFromSurface(renderer, permit_surf);
                    SDL_FreeSurface(permit_surf);
                    SDL_QueryTexture(permit_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, permit_string, NULL, &startRect);
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

