using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using System;
using System.IO;

namespace Game1_Test
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Game1 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Model[] model_mesh = new Model[100];
        Vector3[] model_pos = new Vector3[100];
        Quaternion[] model_rot = new Quaternion[100];
        Vector3[] model_scale = new Vector3[100];
        Vector3 cam_pos;
        Vector3 cam_rot;

        void load_level(string file)
        {
            //Console.WriteLine("Hello World");
            string[] lines = File.ReadAllLines(file);
            bool isLVL = false;
            int mesh_index = 0;
            foreach (string line in lines)
            {
                //Console.WriteLine("\t" + line);

                if (line == "lvl1")
                {
                    isLVL = true;
                    continue;
                }

                if (line[0] == '#')
                {
                    continue;
                }

                if (isLVL)
                {
                    // parsePos
                    string model, name, tag; //0, 1, 2
                    float x, y, z; //3,4,5
                    float rx, ry, rz, rw; //6,7,8, 9
                    float sx, sy, sz; //10,11,12
                    string[] tmp = line.Split(',');

                    model = tmp[0];
                    name = tmp[1];
                    tag = tmp[2];
                    x = float.Parse(tmp[3], System.Globalization.CultureInfo.InvariantCulture);
                    y = float.Parse(tmp[4], System.Globalization.CultureInfo.InvariantCulture);
                    z = float.Parse(tmp[5], System.Globalization.CultureInfo.InvariantCulture);
                    rx = float.Parse(tmp[6], System.Globalization.CultureInfo.InvariantCulture);
                    ry = float.Parse(tmp[7], System.Globalization.CultureInfo.InvariantCulture);
                    rz = float.Parse(tmp[8], System.Globalization.CultureInfo.InvariantCulture);
                    rw = float.Parse(tmp[9], System.Globalization.CultureInfo.InvariantCulture);
                    sx = float.Parse(tmp[10], System.Globalization.CultureInfo.InvariantCulture);
                    sy = float.Parse(tmp[11], System.Globalization.CultureInfo.InvariantCulture);
                    sz = float.Parse(tmp[12], System.Globalization.CultureInfo.InvariantCulture);

                    if (tag == "MainCamera")
                    {
                        cam_pos = new Vector3(-x, y, z);
                        cam_rot = new Vector3(rx, ry, rz);
                        continue;
                    }

                    if (model == "null")
                        continue;

                    model_mesh[mesh_index] = Content.Load<Model>(model.Split('.')[0]);
                    model_pos[mesh_index] = new Vector3(-x,y,z);
                    model_rot[mesh_index] = new Quaternion(rx,-ry,-rz,rw);
                    model_scale[mesh_index] = new Vector3(sx,sy,sz);

                    Console.WriteLine("OUTPUT:");
                    Console.WriteLine($" model: {model}\n name:  {name}\n tag: {tag}");
                    Console.WriteLine($" pos:  {x} {y} {z}");
                    Console.WriteLine($" rot: {rx} {ry} {rz} {rw}");
                    Console.WriteLine($" scale: {sx} {sy} {sz} ");
                }
                mesh_index++;
            }
        }

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            //load_level("SampleScene.lvl");
            load_level("SampleScene.lvl");
            //model_mesh[0] = Content.Load<Model>("cube");
            //model_mesh[1] = Content.Load<Model>("cube");
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// game-specific content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        private void DrawModel(Model m, Vector3 pos, Vector3 rot)
        {
            Matrix[] transforms = new Matrix[m.Bones.Count];
            float aspectRatio = graphics.GraphicsDevice.Viewport.Width / graphics.GraphicsDevice.Viewport.Height;
            m.CopyAbsoluteBoneTransformsTo(transforms);
            Matrix projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(70.0f), aspectRatio, 1.0f, 10000.0f);
            Matrix view = Matrix.CreateLookAt(new Vector3(0.0f, 0.0f, 0.0f), Vector3.Zero, Vector3.Up);

            foreach (ModelMesh mesh in m.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.EnableDefaultLighting();

                    effect.View = view;
                    effect.Projection = projection;
                    Matrix world_rot = Matrix.CreateRotationX(MathHelper.ToRadians(rot.X)) 
                        * Matrix.CreateRotationX(MathHelper.ToRadians(rot.Y))
                        * Matrix.CreateRotationX(MathHelper.ToRadians(rot.Z));
                    effect.World = world_rot * transforms[mesh.ParentBone.Index] * Matrix.CreateTranslation(pos);
                }
                mesh.Draw();
            }
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        /// 
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
           for (int m = 0; m < model_mesh.Length; m++)
            {
                if(model_mesh[m] == null)
                    continue;

                foreach (var mesh in model_mesh[m].Meshes)
                {
                    foreach (var effect1 in mesh.Effects)
                    {
                        var effect = (BasicEffect)effect1;
                        effect.EnableDefaultLighting();
                        effect.PreferPerPixelLighting = true;
                        //effect.World = Matrix.CreateWorld(new Vector3(model_pos[m].X, model_pos[m].Y, model_pos[m].Z), new Vector3(0, 0, -1), Vector3.Up);
                        // https://docs.unity3d.com/ScriptReference/Transform-eulerAngles.html

                        effect.World = Matrix.CreateScale(model_scale[m].X, model_scale[m].Y, model_scale[m].Z)
                            * Matrix.CreateFromQuaternion(model_rot[m])
                            * Matrix.CreateTranslation(model_pos[m].X, model_pos[m].Y, model_pos[m].Z);


                        var cameraPosition = cam_pos;
                        var cameraLookAtVector = cam_rot;
                        var cameraUpVector = Vector3.UnitY;
                        effect.View = Matrix.CreateLookAt(cameraPosition, cameraLookAtVector, cameraUpVector);

                        float aspectRatio = graphics.PreferredBackBufferWidth / (float)graphics.PreferredBackBufferHeight;
                        float fieldOfView = MathHelper.ToRadians(60f);
                        float nearClipPlane = 0.01f;
                        float farClipPlane = 1000f;
                        effect.Projection = Matrix.CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearClipPlane, farClipPlane);
                    }
                    mesh.Draw();
                }
            }

            //DrawModel(model_mesh[0], model_pos[0], model_rot[0]);
            //DrawModel(model_mesh[1], model_pos[1], model_rot[1]);

            base.Draw(gameTime);
        }
    }
}
