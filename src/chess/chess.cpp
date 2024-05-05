#include "chess.h"

Chess::Chess(const Config& config, const std::string& chess_name, const int& origin_x, const int& origin_y, const int& lattice_size)
{
    std::string temp;
    m_chess_name = chess_name;
    m_chess_resource_path = config.Read(chess_name+"_resource_path", temp);
    m_chess_multiple = config.Read(chess_name+"_multiple", 0.0);
    // this->chess_origin_size = config.Read(chess_name+"_origin_size", 0);  // 棋子大小从棋子图片获取，取消从配置文件读取
    m_origin_x = origin_x;
    m_origin_y = origin_y;
    m_lattice_size = lattice_size;
    m_sdl_texture = new SDLTexture(m_chess_name);
    m_is_load_resource = false;
    INFOLOG("Chess1 construct success||chess_resource_path={}||chess_name={}||chess_multiple={}", 
    m_chess_resource_path, m_chess_name, m_chess_multiple);
}

Chess::Chess(const Config& config, const std::string& chess_name)
{
    std::string temp;
    m_chess_name = chess_name;
    m_chess_resource_path = config.Read(chess_name+"_resource_path", temp);
    // this->chess_origin_size = config.Read(chess_name+"_origin_size", 0);
    m_sdl_texture = new SDLTexture(m_chess_name);
    m_is_load_resource = false;
    INFOLOG("Chess2 construct success||chess_resource_path={}||chess_name={}||chess_multiple={}", 
    m_chess_resource_path, m_chess_name, m_chess_multiple);

}

Chess::~Chess()
{
    m_sdl_texture->free();
    delete m_sdl_texture;
    INFOLOG("~Chess, release resource||chess_name={}", m_chess_name);
}

void Chess::init(SDLWindow* chess_window)
{
    m_chess_window = chess_window;

    //Load data
    if(!m_sdl_texture->loadPixelsFromFile(m_chess_window->getWindow(), m_chess_resource_path))
    {        
        ERRORLOG("Failed to load texture!");
        return;
    }
    // 获取棋子图片的原始尺寸
    m_chess_origin_width = m_sdl_texture->getWidth();
    m_chess_origin_height = m_sdl_texture->getHeight();
    // INFOLOG("load texture success!");
    //Get pixel data
    Uint32* pixels = m_sdl_texture->getPixels32();
    int pixelCount = m_sdl_texture->getPitch32() * m_sdl_texture->getHeight();
    //Map colors
    Uint32 colorKey = 28095;   //取出棋子周围的颜色，用下面的值将其设置为透明色
    Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(m_chess_window->getWindow())->format, 0xFF, 0xFF, 0xFF, 0x00);
    // INFOLOG("loadMedia||pixelCount={}||colorKey={}||transparent={}", pixelCount, std::to_string(colorKey), std::to_string(transparent));
    //Color key pixels
    for(int i = 0; i < pixelCount; ++i)
    {
        // DEBUGLOG("loadMedia||pixelCount={}", pixels[i]);
        if(pixels[i] == colorKey)
        {
            pixels[i] = transparent;
        }
    }
    //Create texture from manually color keyed pixels
    if(!m_sdl_texture->loadFromPixels(m_chess_window->getRenderer()))
    {
        ERRORLOG("Unable to load texture from surface!");
        return;
    }
    m_is_load_resource = true;
    // DEBUGLOG("loadResource success||name={}", m_chess_name);
    INFOLOG("init||Chessboard init success||load resource success");
}

bool Chess::chessRender(const int& x, const int& y)
{
    if (!m_is_load_resource)
    {
        ERRORLOG("chess not load resource, please load resource||name={}", m_chess_name);
        return false;
    }
    
    int new_width = (int)m_chess_origin_width * m_chess_multiple;
    int new_height = (int)m_chess_origin_height * m_chess_multiple;
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int new_chess_x = x * m_lattice_size + m_origin_x - x_offset;
    int new_chess_y = y * m_lattice_size + m_origin_y - y_offset;
    m_sdl_texture->render(m_chess_window->getRenderer(), new_chess_x, new_chess_y, m_chess_multiple);
    // DEBUGLOG("chessRender||chess_name={}||x={}||y={}||chess_x={}||chess_y={}",m_chess_name, x, y, chess_x, chess_y);
    return true;
}

bool Chess::chessRender()
{
    if (!m_is_load_resource)
    {
        ERRORLOG("chess not load resource, please load resource||name={}", m_chess_name);
        return false;
    }
    int new_width = (int)m_chess_origin_width * m_chess_multiple;
    int new_height = (int)m_chess_origin_height * m_chess_multiple;
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int new_chess_x = m_chess_x - x_offset;
    int new_chess_y = m_chess_y - y_offset;
    m_sdl_texture->render(m_chess_window->getRenderer(), new_chess_x, new_chess_y, m_chess_multiple);
    // DEBUGLOG("chessRender||chess_name={}||chess_x={}||chess_y={}",m_chess_name, m_chess_x, m_chess_y);
    return true;
}

void Chess::setChessCoordinate(const int& x, const int& y)
{
    m_chess_x = x;
    m_chess_y = y;
    // INFOLOG("setChessCoordinate||chess_x={}||chess_y={}", m_chess_x, m_chess_y);
}

std::pair<int, int> Chess::getChessCoordinate()
{
    std::pair<int, int> chess_coordinate = std::make_pair(m_chess_x, m_chess_y);
    // DEBUGLOG("getChessCoordinate||chess_x={}||chess_y={}", chess_coordinate.first, chess_coordinate.second);
    return chess_coordinate;
}

void Chess::setChessMultiple(const float& multiple)
{
    m_chess_multiple = multiple;
    // INFOLOG("setChessMultiple||chess_multiple{}", m_chess_multiple);
}

int Chess::getChessSize()
{
    int chess_size = m_chess_origin_width > m_chess_origin_height ? m_chess_origin_width : m_chess_origin_height;
    return (int)chess_size * m_chess_multiple;
}
