#include "raylib.h"
#include <vector>

struct Coin
{
    Rectangle rect;
    bool collected;
};

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "My Game");
    SetTargetFPS(60);

    Rectangle player = {100, 100, 40, 50};

    float playerSpeed = 5.0f;
    float jumpForce = -12.0f;
    float gravity = 0.6f;
    float velocityY = 0.0f;

    bool onGround = false;
    int score = 0;

    std::vector<Rectangle> platforms =
    {
        {0, 650, 2000, 70},
        {250, 550, 200, 20},
        {550, 450, 200, 20},
        {850, 350, 200, 20},
        {1200, 500, 250, 20},
        {1600, 400, 250, 20}
    };

    std::vector<Coin> coins =
    {
        {{320, 500, 20, 20}, false},
        {{620, 400, 20, 20}, false},
        {{920, 300, 20, 20}, false},
        {{1300, 450, 20, 20}, false},
        {{1700, 350, 20, 20}, false}
    };

    Camera2D camera = {0};
    camera.target = {player.x, player.y};
    camera.offset = {(float)screenWidth / 2, (float)screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool victory = false;

    while (!WindowShouldClose())
    {
        if (!victory)
        {
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                player.x -= playerSpeed;
            }

            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                player.x += playerSpeed;
            }

            if ((IsKeyPressed(KEY_SPACE) ||
                 IsKeyPressed(KEY_W) ||
                 IsKeyPressed(KEY_UP)) &&
                 onGround)
            {
                velocityY = jumpForce;
                onGround = false;
            }

            velocityY += gravity;
            player.y += velocityY;

            onGround = false;

            for (size_t i = 0; i < platforms.size(); i++)
            {
                Rectangle p = platforms[i];

                if (CheckCollisionRecs(player, p))
                {
                    float playerBottom = player.y + player.height;

                    if (velocityY > 0 &&
                        playerBottom > p.y &&
                        player.y < p.y)
                    {
                        player.y = p.y - player.height;
                        velocityY = 0;
                        onGround = true;
                    }
                }
            }

            for (size_t i = 0; i < coins.size(); i++)
            {
                if (!coins[i].collected)
                {
                    if (CheckCollisionRecs(player, coins[i].rect))
                    {
                        coins[i].collected = true;
                        score++;
                    }
                }
            }

            if (score == (int)coins.size())
            {
                victory = true;
            }

            if (player.y > 1000)
            {
                player.x = 100;
                player.y = 100;
                velocityY = 0;
            }

            camera.target =
            {
                player.x + player.width / 2,
                player.y + player.height / 2
            };
        }

        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode2D(camera);

        DrawRectangle(-5000, 700, 10000, 1000, DARKGREEN);

        for (size_t i = 0; i < platforms.size(); i++)
        {
            DrawRectangleRec(platforms[i], BROWN);
        }

        for (size_t i = 0; i < coins.size(); i++)
        {
            if (!coins[i].collected)
            {
                DrawCircle(
                    (int)(coins[i].rect.x + 10),
                    (int)(coins[i].rect.y + 10),
                    10,
                    GOLD
                );
            }
        }

        DrawRectangleRec(player, RED);

        DrawText(
            "MY GAME",
            50,
            50,
            40,
            DARKBLUE
        );

        EndMode2D();

        DrawRectangle(0, 0, screenWidth, 50, Fade(BLACK, 0.3f));

        DrawText(
            TextFormat("Moedas: %i / %i",
            score,
            (int)coins.size()),
            20,
            15,
            20,
            WHITE
        );

        DrawText(
            "A/D ou Setas = Mover",
            350,
            15,
            20,
            WHITE
        );

        DrawText(
            "ESPACO = Pular",
            650,
            15,
            20,
            WHITE
        );

        if (victory)
        {
            DrawRectangle(
                0,
                0,
                screenWidth,
                screenHeight,
                Fade(BLACK, 0.6f)
            );

            DrawText(
                "VOCE VENCEU!",
                screenWidth / 2 - 220,
                screenHeight / 2 - 60,
                60,
                YELLOW
            );

            DrawText(
                "Todas as moedas foram coletadas",
                screenWidth / 2 - 220,
                screenHeight / 2 + 20,
                30,
                WHITE
            );

            DrawText(
                "Pressione ESC para sair",
                screenWidth / 2 - 170,
                screenHeight / 2 + 80,
                25,
                LIGHTGRAY
            );
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
