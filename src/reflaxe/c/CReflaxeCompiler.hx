package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.GenericCompiler;
import reflaxe.data.ClassFuncData;
import reflaxe.data.ClassVarData;
import reflaxe.data.EnumOptionData;
import reflaxe.output.DataAndFileInfo;
import reflaxe.output.StringOrBytes;
import reflaxe.c.emit.GeneratedFile;

/** Reflaxe adapter. Semantic lowering remains in `CCompiler`. */
class CReflaxeCompiler extends GenericCompiler<Bool, Bool, Dynamic, Dynamic, Dynamic> {
  var allModules:Array<ModuleType> = [];
  var generatedFiles:Array<GeneratedFile> = [];
  var compilationContext:Null<CompilationContext> = null;

  public function new() {
    super();
  }

  override public function filterTypes(moduleTypes:Array<ModuleType>):Array<ModuleType> {
    allModules = moduleTypes.copy();
    return moduleTypes;
  }

  override public function onCompileStart():Void {
    // Every compilation gets fresh mutable state. Never cache a context across
    // compiler-server requests.
    compilationContext = new CompilationContext(ProfileResolver.resolve());
    generatedFiles = [];
  }

  override public function onCompileEnd():Void {
    final context = requireContext();
    generatedFiles = new CCompiler(context).compileModules(allModules);
  }

  override public function generateFilesManually():Void {
    if (output == null) {
      Context.fatalError("HXC9000: Reflaxe output manager is not initialized", Context.currentPos());
      return;
    }

    for (file in generatedFiles) {
      output.saveFile(file.relativePath, file.contents);
    }
  }

  public function generateOutputIterator():Iterator<DataAndFileInfo<StringOrBytes>> {
    final empty:Array<DataAndFileInfo<StringOrBytes>> = [];
    return empty.iterator();
  }

  public function compileClassImpl(
    classType:ClassType,
    varFields:Array<ClassVarData>,
    funcFields:Array<ClassFuncData>
  ):Null<Bool> {
    // Selection is intentionally deferred to the whole-program compiler so it
    // can reason about layout, initialization order, runtime slices, and ABI.
    return null;
  }

  public function compileEnumImpl(enumType:EnumType, options:Array<EnumOptionData>):Null<Bool> {
    return null;
  }

  public function compileExpressionImpl(expr:TypedExpr, topLevel:Bool):Null<Dynamic> {
    return null;
  }

  function requireContext():CompilationContext {
    if (compilationContext == null) {
      Context.fatalError("HXC9000: missing per-build CompilationContext", Context.currentPos());
    }
    return cast compilationContext;
  }
}
#else
class CReflaxeCompiler {
  public function new() {}
}
#end
