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
void listDirectory(std::string dirname, int loop, SDL_Rect startRect, SDL_Renderer *renderer, AppData *data_ptr);

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

        //render(renderer, &data);
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

void listDirectory(std::string dirname, int loop, SDL_Rect startRect, SDL_Renderer *renderer, AppData *data_ptr)
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
                    startRect.y += 25;
                    startRect.x -= 30;
                    //if (file_list[i] != "." && file_list[i] != "..") {
                        //listDirectory(full_path, loop+1, textRectNew, renderer);
                    //}
                    
                } else if ((S_IEXEC & file_info.st_mode) != 0) {
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
                    startRect.y += 25;
                    startRect.x -= 30;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "jpeg" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "png" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tif" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "tiff" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "gif") {
                    SDL_RenderCopy(renderer, data_ptr->image, NULL, &startRect);
                    startRect.x += 30;
                    SDL_Color color = {0, 0, 0};
                    SDL_Surface *file_surf = TTF_RenderText_Solid(data_ptr->font, file_list[i].c_str(), color);
                    SDL_Texture *file_string= SDL_CreateTextureFromSurface(renderer, file_surf);
                    SDL_FreeSurface(file_surf);
                    SDL_QueryTexture(file_string, NULL, NULL, &(startRect.w), &(startRect.h));
                    SDL_RenderCopy(renderer, file_string, NULL, &startRect);
                    startRect.w = 20;
                    startRect.h = 20;
                    startRect.y += 25;
                    startRect.x -= 30;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mp4" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mov" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "mkv" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "avi" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "webm") {
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
                    startRect.y += 25;
                    startRect.x -= 30;
                } else if (file_list[i].substr(file_list[i].find_last_of(".") + 1) == "h" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "c" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "cpp" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "py" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "java" || file_list[i].substr(file_list[i].find_last_of(".") + 1) == "js") {
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
                    startRect.y += 25;
                    startRect.x -= 30;
                } else {
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
                    startRect.y += 25;
                    startRect.x -= 30;
                }
                SDL_RenderPresent(renderer);
            }
            
        }
    }
    else {
        fprintf(stderr, "Error: directory '%s' not found\n", dirname.c_str());
    }
}

